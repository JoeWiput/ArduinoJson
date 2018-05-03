// ArduinoJson - arduinojson.org
// Copyright Benoit Blanchon 2014-2018
// MIT License

#pragma once

namespace ArduinoJson {
namespace Internals {
template <typename TReader>
JsonError skipSpacesAndComments(TReader& reader) {
  for (;;) {
    if (reader.ended()) return JsonError::IncompleteInput;
    switch (reader.current()) {
      case '\0':
        return JsonError::IncompleteInput;

      // spaces
      case ' ':
      case '\t':
      case '\r':
      case '\n':
        reader.move();
        continue;

      // comments
      case '/':
        reader.move();  // skip '/'
        switch (reader.current()) {
          // block comment
          case '*': {
            reader.move();  // skip '*'
            bool wasStar = false;
            for (;;) {
              if (reader.current() == '\0') return JsonError::IncompleteInput;
              if (reader.ended()) return JsonError::IncompleteInput;
              if (reader.current() == '/' && wasStar) {
                reader.move();
                break;
              }
              wasStar = reader.current() == '*';
              reader.move();
            }
            break;
          }

          // trailing comment
          case '/':
            // not need to skip "//"
            for (;;) {
              reader.move();
              if (reader.current() == '\0') return JsonError::IncompleteInput;
              if (reader.ended()) return JsonError::IncompleteInput;
              if (reader.current() == '\n') break;
            }
            break;

          // not a comment, just a '/'
          default:
            return JsonError::InvalidInput;
        }
        break;

      default:
        return JsonError::Ok;
    }
  }
}
}  // namespace Internals
}  // namespace ArduinoJson
