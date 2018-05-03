// ArduinoJson - arduinojson.org
// Copyright Benoit Blanchon 2014-2018
// MIT License

#pragma once

#if ARDUINOJSON_ENABLE_PROGMEM

namespace ArduinoJson {
namespace Internals {
class UnsafeFlashStringReader {
  const char* _ptr;

 public:
  explicit UnsafeFlashStringReader(const __FlashStringHelper* ptr)
      : _ptr(reinterpret_cast<const char*>(ptr)) {}

  void move() {
    _ptr++;
  }

  char current() const {
    return pgm_read_byte_near(_ptr);
  }

  bool ended() const {
    // this reader cannot detect the end
    return false;
  }
};

inline UnsafeFlashStringReader makeReader(TChar* input) {
  return UnsafeFlashStringReader(input);
}
}  // namespace Internals
}  // namespace ArduinoJson

#endif
