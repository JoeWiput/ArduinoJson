// ArduinoJson - arduinojson.org
// Copyright Benoit Blanchon 2014-2018
// MIT License

#pragma once

#include "./StringWriter.hpp"

namespace ArduinoJson {
namespace Internals {

template <typename TJsonBuffer>
class JsonBufferWriter {
 public:
  JsonBufferWriter(TJsonBuffer& jb) : _jb(&jb) {}

  typedef typename TJsonBuffer::String String;

  String startString() {
    return _jb->startString();
  }

 private:
  TJsonBuffer* _jb;
};

template <typename TJsonBuffer, typename Enable = void>
struct Writer {
  typedef JsonBufferWriter<TJsonBuffer> type;
};

template <typename TChar>
struct Writer<TChar*, typename EnableIf<!IsConst<TChar>::value>::type> {
  typedef StringWriter<TChar> type;
};

template <typename TJsonBuffer>
typename Writer<TJsonBuffer>::type makeWriter(TJsonBuffer& jb) {
  return JsonBufferWriter<TJsonBuffer>(jb);
}

template <typename TChar>
typename Writer<TChar*>::type makeWriter(TChar* input) {
  return typename Writer<TChar*>::type(input);
}
}  // namespace Internals
}  // namespace ArduinoJson
