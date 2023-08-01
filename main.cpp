#include <iostream>
#include <fstream>

#include "JsonParser.h"
#include "JsonDumper.h"

int main() {
    auto *json = JsonParser::parse_file("test.json");

    std::cout << "\nDisplaying parsed json with indent=2\n";
    JsonDumper::dump(std::cout, json, 2);

    std::ofstream ofs("out.json");
    if(ofs.is_open()) {
        JsonDumper::dump(ofs, json, 2);
    }

    delete json;
    return 0;
}
