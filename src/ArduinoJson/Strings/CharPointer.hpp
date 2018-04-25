// ArduinoJson - arduinojson.org
// Copyright Benoit Blanchon 2014-2018
// MIT License

#pragma once

namespace ArduinoJson {
namespace Internals {

template <typename TChar>
struct CharPointerTraits {
  static bool equals(const TChar* str, const char* expected) {
    return strcmp(reinterpret_cast<const char*>(str), expected) == 0;
  }

  static bool is_null(const TChar* str) {
    return !str;
  }

  typedef const char* duplicate_t;

  template <typename Buffer>
  static duplicate_t duplicate(const TChar* str, Buffer* buffer) {
    if (!str) return NULL;
    size_t size = strlen(reinterpret_cast<const char*>(str)) + 1;
    void* dup = buffer->alloc(size);
    if (dup != NULL) memcpy(dup, str, size);
    return static_cast<duplicate_t>(dup);
  }

  static const bool has_append = false;
  static const bool has_equals = true;
  static const bool should_duplicate = !IsConst<TChar>::value;
};

// char*, unsigned char*, signed char*
// const char*, const unsigned char*, const signed char*
template <typename TChar>
struct StringTraits<TChar*, typename EnableIf<IsChar<TChar>::value>::type>
    : CharPointerTraits<TChar> {};
}
}
