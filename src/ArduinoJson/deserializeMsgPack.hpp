// ArduinoJson - arduinojson.org
// Copyright Benoit Blanchon 2014-2018
// MIT License

#pragma once

#include "MsgPack/MsgPackDeserializer.hpp"
#include "Reading/Reader.hpp"
#include "Writing/Writer.hpp"

namespace ArduinoJson {
// MsgPackError deserializeMsgPack(TDocument& doc, TString json);
// TDocument = DynamicJsonDocument, StaticJsonDocument
// TString = const std::string&, const String&
template <typename TDocument, typename TString>
typename Internals::EnableIf<!Internals::IsArray<TString>::value,
                             MsgPackError>::type
deserializeMsgPack(TDocument &doc, const TString &json) {
  using namespace Internals;
  return makeMsgPackDeserializer(&doc.buffer(), makeReader(json),
                                 makeWriter(doc.buffer()), doc.nestingLimit)
      .parse(doc.template to<JsonVariant>());
}
//
// MsgPackError deserializeMsgPack(TDocument& doc, TString json);
// TDocument = DynamicJsonDocument, StaticJsonDocument
// TString = char*
template <typename TDocument, typename TString>
MsgPackError deserializeMsgPack(TDocument &doc, TString *json) {
  using namespace Internals;
  return makeMsgPackDeserializer(&doc.buffer(), makeReader(json),
                                 makeWriter(json), doc.nestingLimit)
      .parse(doc.template to<JsonVariant>());
}
//
// MsgPackError deserializeMsgPack(TDocument& doc, TString json);
// TDocument = DynamicJsonDocument, StaticJsonDocument
// TString = const char*, const FlashStringHelper*
template <typename TDocument, typename TString>
MsgPackError deserializeMsgPack(TDocument &doc, const TString *json) {
  using namespace Internals;
  return makeMsgPackDeserializer(&doc.buffer(), makeReader(json),
                                 makeWriter(doc.buffer()), doc.nestingLimit)
      .parse(doc.template to<JsonVariant>());
}
//
// MsgPackError deserializeMsgPack(TDocument& doc, TString json);
// TDocument = DynamicJsonDocument, StaticJsonDocument
// TString = std::istream&, Stream&
template <typename TDocument, typename TString>
MsgPackError deserializeMsgPack(TDocument &doc, TString &json) {
  using namespace Internals;
  return makeMsgPackDeserializer(&doc.buffer(), makeReader(json),
                                 makeWriter(doc.buffer()), doc.nestingLimit)
      .parse(doc.template to<JsonVariant>());
}
}  // namespace ArduinoJson
