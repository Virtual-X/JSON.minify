# JSON-minify

A port of the `JSON-minify` utility to the JavaScript language.

## Overview

`JSON-minify` minifies blocks of JSON-like content into valid JSON by removing all whitespace *and* JS-style comments (single-line `//` and multiline `/* .. */`).

With `JSON-minify`, you can maintain developer-friendly JSON documents, but minify them before parsing or transmitting them over-the-wire.

## Testing

Compile the tests with the following command:

```
g++ tests.cpp JSON_minify.cpp
```

Then run the generated executable.

## License

The code and all the documentation are released under the MIT license.

http://getify.mit-license.org/
