// ArduinoJson - arduinojson.org
// Copyright Benoit Blanchon 2014-2018
// MIT License

#pragma once

#if ARDUINOJSON_ENABLE_PROGMEM

namespace ArduinoJson {
namespace Internals {
class FlashStringReader {
  const char* _ptr;

 public:
  FlashStringReader(const __FlashStringHelper* ptr)
      : _ptr(reinterpret_cast<const char*>(ptr)) {}

  void move() {
    _ptr++;
  }

  char current() const {
    return pgm_read_byte_near(_ptr);
  }
};
}  // namespace Internals
}  // namespace ArduinoJson

#endif
