// ArduinoJson - arduinojson.org
// Copyright Benoit Blanchon 2014-2018
// MIT License

#pragma once

#if ARDUINOJSON_ENABLE_STD_STREAM

#include <istream>

namespace ArduinoJson {
namespace Internals {

class StdStreamReader {
  std::istream& _stream;
  char _current, _next;

 public:
  StdStreamReader(std::istream& stream) : _stream(stream), _current(0), _next(0) {}

  void move() {
    _current = _next;
    _next = 0;
  }

  char current() {
    if (!_current) _current = read();
    return _current;
  }

  char next() {
    // assumes that current() has been called
    if (!_next) _next = read();
    return _next;
  }

 private:
  StdStreamReader& operator=(const StdStreamReader&);  // Visual Studio C4512

  char read() {
    return _stream.eof() ? '\0' : static_cast<char>(_stream.get());
  }
};
}
}

#endif
