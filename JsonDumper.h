//
// Created by isura on 2/08/2023.
//

#ifndef JSONPARSER_JSONDUMPER_H
#define JSONPARSER_JSONDUMPER_H

#include <ostream>

#include "Json.h"


struct JsonDumper {
    static void dump(std::ostream &os, const Json *json, size_t indent_spaces);

private:
    static size_t current;
    static void increment_indent();
    static void decrement_indent();
    static void indent(std::ostream &os, size_t indent_spaces);
    static void dump_json_value(std::ostream &os, const JsonValue *json_value, size_t indent_spaces);
};


#endif //JSONPARSER_JSONDUMPER_H
