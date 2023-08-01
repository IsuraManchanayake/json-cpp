//
// Created by isura on 1/08/2023.
//

#include <iomanip>

#include "Json.h"

JsonValue::~JsonValue() noexcept {
    switch (kind) {
        case JsonValueKind::Null:
        case JsonValueKind::Boolean:
        case JsonValueKind::Integer:
        case JsonValueKind::Float:
        case JsonValueKind::String:
            break;
        case JsonValueKind::Array:
            for (auto &elem: std::get<std::vector<JsonValue *>>(value)) {
                delete elem;
            }
            break;
        case JsonValueKind::Map:
            for (auto &pair: std::get<std::unordered_map<std::string, JsonValue *>>(value)) {
                delete pair.second;
            }
            break;
    }
}

JsonValue::JsonValue() : kind(JsonValueKind::Null), value(static_cast<std::nullptr_t>(nullptr)) {
}

std::ostream& operator<<(std::ostream& os, const JsonValue& json_value) {
    switch (json_value.kind) {
        case JsonValueKind::Null:
            os << "null";
            break;
        case JsonValueKind::Boolean:
            os << std::boolalpha << std::get<bool>(json_value.value);
            break;
        case JsonValueKind::Integer:
            os << std::get<int64_t>(json_value.value);
            break;
        case JsonValueKind::Float:
            os << std::get<double>(json_value.value);
            break;
        case JsonValueKind::String:
            os << std::get<std::string>(json_value.value);
            break;
        case JsonValueKind::Array:
            os << "[";
            for (auto &elem: std::get<std::vector<JsonValue *>>(json_value.value)) {
                os << *elem << ", ";
            }
            os << "]";
            break;
        case JsonValueKind::Map:
            os << "{";
            for (auto &pair: std::get<std::unordered_map<std::string, JsonValue *>>(json_value.value)) {
                os << pair.first << ": " << *pair.second << ", ";
            }
            os << "}";
            break;
    }
    return os;
}


Json::Json(JsonKind kind) : kind(kind), object() {
}

Json::~Json() noexcept {
    switch (kind) {
        case JsonKind::Array:
            for (auto &elem: std::get<std::vector<JsonValue *>>(object)) {
                delete elem;
            }
            break;
        case JsonKind::Map:
            for (auto &pair: std::get<std::unordered_map<std::string, JsonValue *>>(object)) {
                delete pair.second;
            }
            break;
    }
}

std::ostream &operator<<(std::ostream &os, const Json &json) {
    switch (json.kind) {
        case JsonKind::Array:
            os << "[";
            for (auto &elem: std::get<std::vector<JsonValue *>>(json.object)) {
                os << *elem << ", ";
            }
            os << "]";
            break;
        case JsonKind::Map:
            os << "{";
            for (auto &pair: std::get<std::unordered_map<std::string, JsonValue *>>(json.object)) {
                os << pair.first << ": " << *pair.second << ", ";
            }
            os << "}";
            break;
    }
    return os;
}