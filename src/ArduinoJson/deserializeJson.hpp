// ArduinoJson - arduinojson.org
// Copyright Benoit Blanchon 2014-2018
// MIT License

#pragma once

#include "Json/Deserialization/JsonDeserializer.hpp"
#include "Reading/Reader.hpp"
#include "Writing/Writer.hpp"

namespace ArduinoJson {
// JsonError deserializeJson(TDocument& doc, TString json);
// TDocument = DynamicJsonDocument, StaticJsonDocument
// TString = const std::string&, const String&
template <typename TDocument, typename TString>
typename Internals::EnableIf<!Internals::IsArray<TString>::value,
                             JsonError>::type
deserializeJson(TDocument &doc, const TString &json) {
  using namespace Internals;
  return makeJsonDeserializer(&doc.buffer(), makeReader(json),
                              makeWriter(doc.buffer()), doc.nestingLimit)
      .parse(doc.template to<JsonVariant>());
}
//
// JsonError deserializeJson(TDocument& doc, TString json);
// TDocument = DynamicJsonDocument, StaticJsonDocument
// TString = char*
template <typename TDocument, typename TString>
JsonError deserializeJson(TDocument &doc, TString *json) {
  using namespace Internals;
  return makeJsonDeserializer(&doc.buffer(), makeReader(json), makeWriter(json),
                              doc.nestingLimit)
      .parse(doc.template to<JsonVariant>());
}
//
// JsonError deserializeJson(TDocument& doc, TString json);
// TDocument = DynamicJsonDocument, StaticJsonDocument
// TString = const char*, const FlashStringHelper*
template <typename TDocument, typename TString>
JsonError deserializeJson(TDocument &doc, const TString *json) {
  using namespace Internals;
  return makeJsonDeserializer(&doc.buffer(), makeReader(json),
                              makeWriter(doc.buffer()), doc.nestingLimit)
      .parse(doc.template to<JsonVariant>());
}
//
// JsonError deserializeJson(TDocument& doc, TString json);
// TDocument = DynamicJsonDocument, StaticJsonDocument
// TString = std::istream&, Stream&
template <typename TDocument, typename TString>
JsonError deserializeJson(TDocument &doc, TString &json) {
  using namespace Internals;
  return makeJsonDeserializer(&doc.buffer(), makeReader(json),
                              makeWriter(doc.buffer()), doc.nestingLimit)
      .parse(doc.template to<JsonVariant>());
}
}  // namespace ArduinoJson
