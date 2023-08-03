#include <iostream>
#include <fstream>

#include "JsonParser.h"
#include "JsonDumper.h"

int main() {
    std::ifstream ifs("test.json");
    if(!ifs.is_open()) {
        std::cout << "Error opening file\n";
        std::exit(1);
    }
    auto *json = JsonParser::parse(ifs);

    std::cout << "\nDisplaying parsed json with indent=2\n";
    JsonDumper::dump(std::cout, json, 2);

    std::ofstream ofs("out.json");
    if(!ofs.is_open()) {
        std::cout << "Error opening file\n";
        std::exit(1);
    }
    JsonDumper::dump(ofs, json, 2);

    delete json;
    return 0;
}
