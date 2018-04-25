// ArduinoJson - arduinojson.org
// Copyright Benoit Blanchon 2014-2018
// MIT License

#pragma once

#if ARDUINOJSON_ENABLE_STD_STREAM

#include <istream>

namespace ArduinoJson {
namespace Internals {

struct StdStreamTraits {
  static const bool has_append = false;
  static const bool has_equals = false;
};

template <typename TStream>
struct StringTraits<
    TStream,
    // match any type that is derived from std::istream:
    typename EnableIf<IsBaseOf<
        std::istream, typename RemoveReference<TStream>::type>::value>::type>
    : StdStreamTraits {};
}
}

#endif
