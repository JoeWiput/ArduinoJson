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
    if (_reader.ended()) return JsonError::IncompleteInput;
    read();
    return parseVariant(variant);
  }

 private:
  JsonDeserializer &operator=(const JsonDeserializer &);  // non-copiable

  bool isEnded() {
    return _last == 0;
  }

  void read() {
    if (_reader.ended()) {
      _last = 0;
    } else {
      _last = _reader.current();
      _reader.move();
    }
  }

  FORCE_INLINE bool eat(char charToSkip) {
    if (_last != charToSkip) return false;
    read();
    return true;
  }

  JsonError parseVariant(JsonVariant &variant) {
    JsonError err = skipSpacesAndComments();
    if (err) return err;

    switch (_last) {
      case '[':
        return parseArray(variant);

      case '{':
        return parseObject(variant);

      default:
        return parseValue(variant);
    }
  }

  JsonError parseArray(JsonVariant &variant) {
    if (_nestingLimit == 0) return JsonError::TooDeep;

    JsonArray *array = new (_buffer) JsonArray(_buffer);
    if (!array) return JsonError::NoMemory;
    variant = array;

    // Check opening braket
    if (!eat('[')) return JsonError::InvalidInput;

    // Skip spaces
    JsonError err = skipSpacesAndComments();
    if (err) return err;

    // Empty array?
    if (eat(']')) return JsonError::Ok;

    // Read each value
    for (;;) {
      // 1 - Parse value
      JsonVariant value;
      _nestingLimit--;
      err = parseVariant(value);
      _nestingLimit++;
      if (err) return err;
      if (!array->add(value)) return JsonError::NoMemory;

      // 2 - Skip spaces
      err = skipSpacesAndComments();
      if (err) return err;

      // 3 - More values?
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
    JsonError err = skipSpacesAndComments();
    if (err) return err;

    // Empty object?
    if (eat('}')) return JsonError::Ok;

    // Read each key value pair
    for (;;) {
      // Parse key
      const char *key;
      err = parseString(&key);
      if (err) return err;

      // Skip spaces
      err = skipSpacesAndComments();
      if (err) return err;

      // Colon
      if (!eat(':')) return JsonError::InvalidInput;

      // Parse value
      JsonVariant value;
      _nestingLimit--;
      err = parseVariant(value);
      _nestingLimit++;
      if (err) return err;
      if (!object->set(key, value)) return JsonError::NoMemory;

      // Skip spaces
      err = skipSpacesAndComments();
      if (err) return err;

      // More keys/values?
      if (eat('}')) return JsonError::Ok;
      if (!eat(',')) return JsonError::InvalidInput;

      // Skip spaces
      err = skipSpacesAndComments();
      if (err) return err;
    }
  }

  JsonError parseValue(JsonVariant &variant) {
    bool hasQuotes = isQuote(_last);
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

    char c = _last;
    if (c == '\0') return JsonError::IncompleteInput;

    if (isQuote(c)) {  // quotes
      read();
      char stopChar = c;
      for (;;) {
        c = _last;
        read();
        if (c == stopChar) break;

        if (isEnded()) return JsonError::IncompleteInput;

        if (c == '\\') {
          if (isEnded()) return JsonError::IncompleteInput;
          // replace char
          c = Encoding::unescapeChar(_last);
          if (c == '\0') return JsonError::InvalidInput;
          read();
        }

        str.append(c);
      }
    } else if (canBeInNonQuotedString(c)) {  // no quotes
      do {
        str.append(c);
        read();
        c = _last;
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

  JsonError skipSpacesAndComments() {
    for (;;) {
      if (isEnded()) return JsonError::IncompleteInput;
      switch (_last) {
        // spaces
        case ' ':
        case '\t':
        case '\r':
        case '\n':
          read();
          break;

        // comments
        case '/':
          read();  // skip '/'
          switch (_last) {
            // block comment
            case '*': {
              read();  // skip '*'
              bool wasStar = false;
              for (;;) {
                if (isEnded()) return JsonError::IncompleteInput;
                if (_last == '/' && wasStar) {
                  read();
                  break;
                }
                wasStar = _last == '*';
                read();
              }
              break;
            }

            // trailing comment
            case '/':
              // not need to skip "//"
              for (;;) {
                read();
                if (isEnded()) return JsonError::IncompleteInput;
                if (_last == '\n') break;
              }
              break;

            // not a comment, just a '/'
            default:
              return JsonError::InvalidInput;
          }
          break;

        default:
          return JsonError::Ok;
      }
    }
  }

  JsonBuffer *_buffer;
  TReader _reader;
  TWriter _writer;
  uint8_t _nestingLimit;
  char _last;
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
