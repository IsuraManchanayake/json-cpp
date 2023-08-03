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

### static Json *JsonParser::parse(std::istream &is)

Reads any C++ style input stream `is` and returns a `Json *`. For example; `std::istringstream`, `std::ifstream`, `std::cin`.

### static void JsonDumper::dump(std::ostream &os, const Json *json, size_t indent_spaces)

Writes the `Json *` to a C++ style output stream `ostream`. For example; `std::ostringstream`, `std::ofstream`, `std::cout`.

## Limitations

- No support for unicode strings
- No exception handling. It just exits when the Json is invalid xD. It's a simple fix, but I'm done with this project.
- Never tested for performance
