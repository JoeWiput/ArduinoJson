// ArduinoJson - arduinojson.org
// Copyright Benoit Blanchon 2014-2018
// MIT License

#pragma once

namespace ArduinoJson {
namespace Internals {

template <typename TChar>
class ZeroTerminatedReader {
  const TChar* _ptr;

 public:
  ZeroTerminatedReader(const TChar* ptr)
      : _ptr(ptr ? ptr : reinterpret_cast<const TChar*>("")) {}

  void move() {
    ++_ptr;
  }

  char current() const {
    return char(_ptr[0]);
  }

  char next() const {
    return char(_ptr[1]);
  }
};
}
}
