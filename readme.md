# JSON Parser

## Compiling

```
$ g++ *.cpp -o main -std=c++2a
```

## Running

```
$ ./main
```

## Methods

### Json *JsonParser::parse_file(const std::string &path)

Reads a json file from `path` and returns a `Json *`.

### Json *JsonParser::parse_string(const std::string &string)

Reads a json string `string` and returns a `Json *`.

### Json *JsonParser::parse_stream(std::istream &istream)

Reads any C++ style input stream `istream` and returns a `Json *`. For example; `std::istringstream`, `std::ifstream`, `std::cin`.

### void JsonDumper::dump(std::ostream &os, const Json *json, size_t indent_spaces)

Writes the `Json *` to a C++ style output stream `ostream`. For example; `std::ostringstream`, `std::ofstream`, `std::cout`.

## Limitations

- No support for unicode strings
- No exception handling. It just exits when the Json is invalid xD. It's a simple fix, but I'm done with this project.
- Never tested for performance
