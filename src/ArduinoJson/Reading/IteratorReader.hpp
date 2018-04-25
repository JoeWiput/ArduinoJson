// ArduinoJson - arduinojson.org
// Copyright Benoit Blanchon 2014-2018
// MIT License

#pragma once

namespace ArduinoJson {
namespace Internals {

template<typename TIterable>
class IteratorReader {
  typename TIterable::const_iterator _ptr;
  typename TIterable::const_iterator const _end;

 public:
  IteratorReader(const TIterable& input)
      : _ptr(input.begin()), _end(input.end()) {}

  void move() {
    if (_ptr != _end)
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
