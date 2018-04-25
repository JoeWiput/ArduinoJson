// ArduinoJson - arduinojson.org
// Copyright Benoit Blanchon 2014-2018
// MIT License

#pragma once

#include "./ArduinoStreamReader.hpp"
#include "./StdStreamReader.hpp"
#include "./ZeroTerminatedReader.hpp"
#include "./IteratorReader.hpp"

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


#ifdef ARDUINOJSON_ENABLE_STD_STREAM
template<typename TStream>
struct Reader<TStream, 
	typename EnableIf<IsBaseOf<std::istream, 
		typename RemoveReference<TStream>::type>::value>::type> {
	typedef StdStreamReader type;
};
#endif
}
}
