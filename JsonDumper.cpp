//
// Created by isura on 2/08/2023.
//

#include <cassert>
#include <iomanip>

#include "JsonDumper.h"

void JsonDumper::dump(std::ostream &os, const Json *json, size_t indent_spaces) {
    assert(json && "Dumping nullptr");
    JsonDumper::current = 0;
    if(json->kind == JsonKind::Map) {
        const auto map = std::get<std::unordered_map<std::string, JsonValue *>>(json->object);
        os << "{\n";
        JsonDumper::increment_indent();
        size_t index = 0;
        for(const auto &[key, value]: map) {
            JsonDumper::indent(os, indent_spaces);
            os << std::quoted(key) << ": ";
            JsonDumper::dump_json_value(os, value, indent_spaces);
            if(index != map.size() - 1) {
                os << ",\n";
            }
            ++index;
        }
        JsonDumper::decrement_indent();
        os << "\n";
        JsonDumper::indent(os, indent_spaces);
        os << "}";
    } else if(json->kind == JsonKind::Array) {
        const auto array = std::get<std::vector<JsonValue *>>(json->object);
        os << "[\n";
        JsonDumper::increment_indent();
        size_t index = 0;
        for(const auto &elem: array) {
            JsonDumper::indent(os, indent_spaces);
            JsonDumper::dump_json_value(os, elem, indent_spaces);
            if(index != array.size() - 1) {
                os << ",\n";
            }
            ++index;
        }
        JsonDumper::decrement_indent();
        os << "\n";
        JsonDumper::indent(os, indent_spaces);
        os << "]";
    } else {
        assert(0);
    }
}

size_t JsonDumper::current = 0;

void JsonDumper::increment_indent() {
    ++JsonDumper::current;
}

void JsonDumper::indent(std::ostream &os, size_t indent_spaces) {
    for(size_t i = 0; i < current * indent_spaces; ++i) {
        os << ' ';
    }
}

void JsonDumper::decrement_indent() {
    --JsonDumper::current;
}

void JsonDumper::dump_json_value(std::ostream &os, const JsonValue *json_value, size_t indent_spaces) {
    switch(json_value->kind) {
        case JsonValueKind::Null:
            os << "null";
            break;
        case JsonValueKind::Boolean:
            os << std::boolalpha << std::get<bool>(json_value->value);
            break;
        case JsonValueKind::Integer:
            os << std::get<int64_t>(json_value->value);
            break;
        case JsonValueKind::Float:
            os << std::get<double>(json_value->value);
            break;
        case JsonValueKind::String:
            os << std::quoted(std::get<std::string>(json_value->value));
            break;
        case JsonValueKind::Array: {
            os << "[\n";
            JsonDumper::increment_indent();
            size_t index = 0;
            for (auto &elem: std::get<std::vector<JsonValue *>>(json_value->value)) {
                JsonDumper::indent(os, indent_spaces);
                JsonDumper::dump_json_value(os, elem, indent_spaces);
                if (index != std::get<std::vector<JsonValue *>>(json_value->value).size() - 1) {
                    os << ",\n";
                }
                ++index;
            }
            JsonDumper::decrement_indent();
            os << "\n";
            JsonDumper::indent(os, indent_spaces);
            os << "]";
            break;
        }
        case JsonValueKind::Map: {
            os << "{\n";
            JsonDumper::increment_indent();
            size_t index = 0;
            for (auto &pair: std::get<std::unordered_map<std::string, JsonValue *>>(json_value->value)) {
                JsonDumper::indent(os, indent_spaces);
                os << std::quoted(pair.first) << ": ";
                JsonDumper::dump_json_value(os, pair.second, indent_spaces);
                if (index != std::get<std::unordered_map<std::string, JsonValue *>>(json_value->value).size() - 1) {
                    os << ",\n";
                }
                ++index;
            }
            JsonDumper::decrement_indent();
            os << "\n";
            JsonDumper::indent(os, indent_spaces);
            os << "}";
            break;
        }
    }
}
