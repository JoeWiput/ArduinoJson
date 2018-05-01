// ArduinoJson - arduinojson.org
// Copyright Benoit Blanchon 2014-2018
// MIT License

#pragma once

#include "../../JsonError.hpp"
#include "../../JsonVariant.hpp"
#include "../../Memory/JsonBuffer.hpp"
#include "../../Reading/Reader.hpp"
#include "../../TypeTraits/IsConst.hpp"
#include "../Encoding.hpp"
#include "./Comments.hpp"

namespace ArduinoJson {
namespace Internals {

template <typename TReader, typename TWriter>
class JsonDeserializer {
 public:
  JsonDeserializer(JsonBuffer *buffer, TReader reader, TWriter writer,
                   uint8_t nestingLimit)
      : _buffer(buffer),
        _reader(reader),
        _writer(writer),
        _nestingLimit(nestingLimit) {}
  JsonError parse(JsonVariant &variant) {
    JsonError err = skipSpacesAndComments(_reader);
    if (err) return err;

    switch (_reader.current()) {
      case '\0':
        return JsonError::IncompleteInput;

      case '[':
        return parseArray(variant);

      case '{':
        return parseObject(variant);

      default:
        return parseValue(variant);
    }
  }

 private:
  JsonDeserializer &operator=(const JsonDeserializer &);  // non-copiable

  FORCE_INLINE bool eat(char charToSkip) {
    if (_reader.current() != charToSkip) return false;
    _reader.move();
    return true;
  }

  JsonError parseArray(JsonVariant &variant) {
    if (_nestingLimit == 0) return JsonError::TooDeep;

    JsonArray *array = new (_buffer) JsonArray(_buffer);
    if (!array) return JsonError::NoMemory;
    variant = array;

    // Check opening braket
    if (!eat('[')) return JsonError::InvalidInput;

    // Skip spaces
    JsonError err = skipSpacesAndComments(_reader);
    if (err) return err;
    if (_reader.ended()) return JsonError::IncompleteInput;

    // Empty array?
    if (eat(']')) return JsonError::Ok;

    // Read each value
    for (;;) {
      // 1 - Parse value
      JsonVariant value;
      _nestingLimit--;
      err = parse(value);
      _nestingLimit++;
      if (err) return err;
      if (!array->add(value)) return JsonError::NoMemory;

      // 2 - Skip spaces
      err = skipSpacesAndComments(_reader);
      if (err) return err;

      // 3 - More values?
      if (_reader.ended()) return JsonError::IncompleteInput;
      if (eat(']')) return JsonError::Ok;
      if (!eat(',')) return JsonError::InvalidInput;
    }
  }

  JsonError parseObject(JsonVariant &variant) {
    if (_nestingLimit == 0) return JsonError::TooDeep;

    JsonObject *object = new (_buffer) JsonObject(_buffer);
    if (!object) return JsonError::NoMemory;
    variant = object;

    // Check opening brace
    if (!eat('{')) return JsonError::InvalidInput;

    // Skip spaces
    JsonError err = skipSpacesAndComments(_reader);
    if (err) return err;
    if (_reader.ended()) return JsonError::IncompleteInput;

    // Empty object?
    if (eat('}')) return JsonError::Ok;

    // Read each key value pair
    for (;;) {
      // Parse key
      const char *key;
      err = parseString(&key);
      if (err) return err;

      // Skip spaces
      err = skipSpacesAndComments(_reader);
      if (err) return err;

      // Colon
      if (!eat(':')) return JsonError::InvalidInput;

      // Parse value
      JsonVariant value;
      _nestingLimit--;
      err = parse(value);
      _nestingLimit++;
      if (err) return err;
      if (!object->set(key, value)) return JsonError::NoMemory;

      // Skip spaces
      err = skipSpacesAndComments(_reader);
      if (err) return err;

      // More keys/values?
      if (_reader.ended()) return JsonError::IncompleteInput;
      if (eat('}')) return JsonError::Ok;
      if (!eat(',')) return JsonError::InvalidInput;

      // Skip spaces
      err = skipSpacesAndComments(_reader);
      if (err) return err;
    }
  }

  JsonError parseValue(JsonVariant &variant) {
    bool hasQuotes = isQuote(_reader.current());
    const char *value;
    JsonError error = parseString(&value);
    if (error) return error;
    if (hasQuotes) {
      variant = value;
    } else {
      variant = RawJson(value);
    }
    return JsonError::Ok;
  }

  JsonError parseString(const char **result) {
    typename RemoveReference<TWriter>::type::String str = _writer.startString();

    char c = _reader.current();

    if (isQuote(c)) {  // quotes
      _reader.move();
      char stopChar = c;
      for (;;) {
        if (_reader.ended()) return JsonError::IncompleteInput;
        c = _reader.current();
        _reader.move();

        if (c == stopChar) break;

        if (c == '\\') {
          // replace char
          c = Encoding::unescapeChar(_reader.current());
          if (c == '\0') return JsonError::InvalidInput;
          _reader.move();
        }

        str.append(c);
      }
    } else if (canBeInNonQuotedString(c)) {  // no quotes
      do {
        _reader.move();
        str.append(c);
        c = _reader.current();
      } while (canBeInNonQuotedString(c));
    } else {
      return JsonError::InvalidInput;
    }

    *result = str.c_str();
    if (*result == NULL) return JsonError::NoMemory;
    return JsonError::Ok;
  }

  static inline bool isBetween(char c, char min, char max) {
    return min <= c && c <= max;
  }

  static inline bool canBeInNonQuotedString(char c) {
    return isBetween(c, '0', '9') || isBetween(c, '_', 'z') ||
           isBetween(c, 'A', 'Z') || c == '+' || c == '-' || c == '.';
  }

  static inline bool isQuote(char c) {
    return c == '\'' || c == '\"';
  }

  JsonBuffer *_buffer;
  TReader _reader;
  TWriter _writer;
  uint8_t _nestingLimit;
};

template <typename TJsonBuffer, typename TReader, typename TWriter>
JsonDeserializer<TReader, TWriter> makeJsonDeserializer(TJsonBuffer *buffer,
                                                        TReader reader,
                                                        TWriter writer,
                                                        uint8_t nestingLimit) {
  return JsonDeserializer<TReader, TWriter>(buffer, reader, writer,
                                            nestingLimit);
}
}  // namespace Internals
}  // namespace ArduinoJson
