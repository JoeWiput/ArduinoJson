// ArduinoJson - arduinojson.org
// Copyright Benoit Blanchon 2014-2018
// MIT License

#pragma once

#include "./ArduinoStreamReader.hpp"
#include "./FlashStringReader.hpp"
#include "./IteratorReader.hpp"
#include "./StdStreamReader.hpp"
#include "./ZeroTerminatedReader.hpp"

namespace ArduinoJson{
namespace Internals {
template<typename TInput, typename Enable = void>
struct Reader {
typedef IteratorReader<TInput> type;
};

template<typename TChar>
struct Reader<TChar*, typename EnableIf<IsChar<TChar>::value>::type > {
	typedef ZeroTerminatedReader<TChar> type;
};

#if ARDUINOJSON_ENABLE_ARDUINO_STREAM
template <typename TStream>
struct Reader<
    TStream,
    // match any type that is derived from Stream:
    typename EnableIf<
        IsBaseOf<Stream, typename RemoveReference<TStream>::type>::value>::type> {
   typedef ArduinoStreamReader type;
}
#endif


#if ARDUINOJSON_ENABLE_STD_STREAM
template<typename TStream>
struct Reader<TStream, 
	typename EnableIf<IsBaseOf<std::istream, 
		typename RemoveReference<TStream>::type>::value>::type> {
	typedef StdStreamReader type;
};
#endif

#if ARDUINOJSON_ENABLE_PROGMEM
template<>
struct Reader<const __FlashStringHelper*, void> {
	typedef FlashStringReader<TChar> type;
};
#endif
}
}
