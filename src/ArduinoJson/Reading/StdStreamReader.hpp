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
  char _current;

 public:
  StdStreamReader(std::istream& stream) : _stream(stream), _current(0) {}

  void move() {
    _current = 0;
  }

  char current() {
    if (!_current) _current = read();
    return _current;
  }

  bool ended() {
    return _stream.eof();
  }

 private:
  StdStreamReader& operator=(const StdStreamReader&);  // Visual Studio C4512

  char read() {
    return _stream.eof() ? '\0' : static_cast<char>(_stream.get());
  }
};
}  // namespace Internals
}  // namespace ArduinoJson

#endif
