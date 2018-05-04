// ArduinoJson - arduinojson.org
// Copyright Benoit Blanchon 2014-2018
// MIT License

#include <ArduinoJson.h>
#include <catch.hpp>

static void checkIncomplete(const char* input, size_t len) {
  DynamicJsonDocument doc;

  MsgPackError error = deserializeMsgPack(doc, input, len);

  REQUIRE(error == MsgPackError::IncompleteInput);
}

TEST_CASE("deserializeMsgPack() returns IncompleteInput") {
  SECTION("empty input") {
    checkIncomplete("\x00", 0);
  }
}
