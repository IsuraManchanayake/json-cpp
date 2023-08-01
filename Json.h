//
// Created by isura on 1/08/2023.
//

#ifndef JSONPARSER_JSON_H
#define JSONPARSER_JSON_H

#include <vector>
#include <string>
#include <cstdint>
#include <unordered_map>
#include <variant>
#include <ostream>

enum class JsonValueKind {
    Null,
    Boolean,
    Integer,
    Float,
    String,
    Array,
    Map,
};

enum class JsonKind {
    Array,
    Map,
};


struct JsonValue {
    using VariantType = std::variant<std::nullptr_t, bool, int64_t, double, std::string, std::vector<JsonValue *>, std::unordered_map<std::string, JsonValue *>>;

    JsonValueKind kind;
    VariantType value;

    template<typename T>
    JsonValue(JsonValueKind kind, const T &value);

    JsonValue();

    ~JsonValue() noexcept;
};


std::ostream &operator<<(std::ostream &os, const JsonValue &json_value);


template<typename T>
JsonValue::JsonValue(JsonValueKind kind, const T &value) : kind(kind), value(value) {
}


struct Json {
    using VariantType = std::variant<std::vector<JsonValue *>, std::unordered_map<std::string, JsonValue *>>;

    JsonKind kind;
    VariantType object;

    explicit Json(JsonKind kind);

    ~Json() noexcept;
};

std::ostream &operator<<(std::ostream &os, const Json &json);

#endif //JSONPARSER_JSON_H
