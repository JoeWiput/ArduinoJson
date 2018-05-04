# ArduinoJson - arduinojson.org
# Copyright Benoit Blanchon 2014-2018
# MIT License

add_executable(MsgPackTests
	deserializeArray.cpp
	deserializeObject.cpp
	deserializeStaticVariant.cpp
	deserializeVariant.cpp
	doubleToFloat.cpp
	MsgPackError.cpp
	nestingLimit.cpp
	notSupported.cpp
)

target_link_libraries(MsgPackTests catch)
add_test(MsgPack MsgPackTests)
