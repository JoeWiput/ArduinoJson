// ArduinoJson - arduinojson.org
// Copyright Benoit Blanchon 2014-2018
// MIT License

#pragma once

namespace ArduinoJson {
namespace Internals {
template <typename TReader>
void skipSpacesAndComments(TReader& reader) {
  for (;;) {
    switch (reader.current()) {
      // spaces
      case ' ':
      case '\t':
      case '\r':
      case '\n':
        reader.move();
        continue;

      // comments
      case '/':
        switch (reader.next()) {
          // C-style block comment
          case '*':
            reader.move();  // skip '/'
            // no need to skip '*'
            for (;;) {
              reader.move();
              if (reader.current() == '\0') return;
              if (reader.current() == '*' && reader.next() == '/') {
                reader.move();  // skip '*'
                reader.move();  // skip '/'
                break;
              }
            }
            break;

          // C++-style line comment
          case '/':
            // not need to skip "//"
            for (;;) {
              reader.move();
              if (reader.current() == '\0') return;
              if (reader.current() == '\n') break;
            }
            break;

          // not a comment, just a '/'
          default:
            return;
        }
        break;

      default:
        return;
    }
  }
}
}
}
