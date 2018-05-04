// ArduinoJson - arduinojson.org
// Copyright Benoit Blanchon 2014-2018
// MIT License

#include <ArduinoJson.h>
#include <catch.hpp>

static void checkComplete(const char* input, size_t len) {
  DynamicJsonDocument doc;

  MsgPackError error = deserializeMsgPack(doc, input, len);

  REQUIRE(error == MsgPackError::Ok);
}

static void checkIncomplete(const char* input, size_t len) {
  DynamicJsonDocument doc;

  MsgPackError error = deserializeMsgPack(doc, input, len);

  REQUIRE(error == MsgPackError::IncompleteInput);
}

TEST_CASE("deserializeMsgPack() returns IncompleteInput") {
  SECTION("empty input") {
    checkIncomplete("\x00", 0);
    checkComplete("\x00", 1);
  }

  SECTION("fixarray") {
    checkIncomplete("\x91\x01", 1);
    checkComplete("\x00", 2);
  }

  SECTION("array 16") {
    checkIncomplete("\xDC\x00\x01\x01", 1);
    checkIncomplete("\xDC\x00\x01\x01", 2);
    checkIncomplete("\xDC\x00\x01\x01", 3);
    checkComplete("\xDC\x00\x01\x01", 4);
  }
}
