// ArduinoJson - arduinojson.org
// Copyright Benoit Blanchon 2014-2018
// MIT License

#pragma once

#include "./ArduinoStreamReader.hpp"
#include "./FlashStringReader.hpp"
#include "./IteratorReader.hpp"
#include "./StdStreamReader.hpp"
#include "./ZeroTerminatedReader.hpp"

namespace ArduinoJson {
namespace Internals {
template <typename TInput, typename Enable = void>
struct Reader {
  typedef IteratorReader<const char*> type;

  static type create(const TInput& input) {
    // Arduino String class has no begin()/end() member until version 1.7
    // and it has no size() function
    return type(input.c_str(), input.c_str() + input.length());
  }
};

template <typename T, typename R = void>
struct Enable {
  typedef R type;
};

template <typename TIterable>
struct Reader<TIterable,
              typename Enable<typename TIterable::const_iterator>::type> {
  typedef IteratorReader<typename TIterable::const_iterator> type;
  static type create(const TIterable& input) {
    return type(input.begin(), input.end());
  }
};

template <typename TChar>
struct Reader<TChar*, typename EnableIf<IsChar<TChar>::value>::type> {
  typedef ZeroTerminatedReader<TChar> type;

  static type create(const TChar* input) {
    return type(input);
  }
};

#if ARDUINOJSON_ENABLE_ARDUINO_STREAM
template <typename TStream>
struct Reader<
    TStream,
    // match any type that is derived from Stream:
    typename EnableIf<IsBaseOf<
        Stream, typename RemoveReference<TStream>::type>::value>::type> {
  typedef ArduinoStreamReader type;

  static type create(TStream& input) {
    return type(input);
  }
};
#endif

#if ARDUINOJSON_ENABLE_STD_STREAM
template <typename TStream>
struct Reader<
    TStream,
    typename EnableIf<IsBaseOf<
        std::istream, typename RemoveReference<TStream>::type>::value>::type> {
  typedef StdStreamReader type;

  static type create(TStream& input) {
    return type(input);
  }
};
#endif

#if ARDUINOJSON_ENABLE_PROGMEM
template <>
struct Reader<const __FlashStringHelper*, void> {
  typedef FlashStringReader type;

  static type create(const __FlashStringHelper* input) {
    return type(input);
  }
};
#endif

template <typename TString>
typename Reader<TString>::type makeReader(TString& input) {
  return Reader<TString>::create(input);
}

template <typename TChar>
typename Reader<TChar*>::type makeReader(TChar* input) {
  return Reader<TChar*>::create(input);
}

}  // namespace Internals
}  // namespace ArduinoJson
