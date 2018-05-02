// ArduinoJson - arduinojson.org
// Copyright Benoit Blanchon 2014-2018
// MIT License

#include <ArduinoJson.h>
#include <catch.hpp>
#include <sstream>

TEST_CASE("serialize to std::stream") {
  DynamicJsonDocument doc;
  std::ostringstream os;

  SECTION("JsonVariant containing false") {
    JsonVariant variant = false;

    os << variant;

    REQUIRE("false" == os.str());
  }

  SECTION("JsonVariant containing string") {
    JsonVariant variant = "coucou";

    os << variant;

    REQUIRE("\"coucou\"" == os.str());
  }

  SECTION("JsonObject") {
    JsonObject& object = doc.to<JsonObject>();
    object["key"] = "value";

    os << object;

    REQUIRE("{\"key\":\"value\"}" == os.str());
  }

  SECTION("JsonObjectSubscript") {
    JsonObject& object = doc.to<JsonObject>();
    object["key"] = "value";

    os << object["key"];

    REQUIRE("\"value\"" == os.str());
  }

  SECTION("JsonArray") {
    JsonArray& array = doc.to<JsonArray>();
    array.add("value");

    os << array;

    REQUIRE("[\"value\"]" == os.str());
  }

  SECTION("JsonArraySubscript") {
    JsonArray& array = doc.to<JsonArray>();
    array.add("value");

    os << array[0];

    REQUIRE("\"value\"" == os.str());
  }
}

TEST_CASE("deserialize from std::stream") {
  DynamicJsonDocument doc;

  SECTION("array") {
    std::istringstream json(" [ 42 /* comment */ ] ");

    JsonError err = deserializeJson(doc, json);
    JsonArray& arr = doc.as<JsonArray>();

    REQUIRE(err == JsonError::Ok);
    REQUIRE(1 == arr.size());
    REQUIRE(42 == arr[0]);
  }

  SECTION("object") {
    std::istringstream json(" { hello : world // comment\n }");

    JsonError err = deserializeJson(doc, json);
    JsonObject& obj = doc.as<JsonObject>();

    REQUIRE(err == JsonError::Ok);
    REQUIRE(1 == obj.size());
    REQUIRE(std::string("world") == obj["hello"]);
  }

  SECTION("Should not read after the closing brace") {
    std::istringstream json("{}123");

    deserializeJson(doc, json);

    REQUIRE('1' == char(json.get()));
  }

  SECTION("Should not read after the closing bracket") {
    std::istringstream json("[]123");

    deserializeJson(doc, json);

    REQUIRE('1' == char(json.get()));
  }
}
