//
// Created by isura on 1/08/2023.
//

#ifndef JSONPARSER_JSONPARSER_H
#define JSONPARSER_JSONPARSER_H

#include <string>
#include <string>
#include <variant>

#include "Json.h"

enum class TokenKind {
    LeftCurly,
    RightCurly,
    LeftSquare,
    RightSquare,
    Colon,
    Comma,
    String,
    Integer,
    Float,
    Boolean,
    Null,
};

struct Token {
    TokenKind kind;
    std::variant<int64_t, double, bool, std::string> value;

    explicit Token(TokenKind kind);
};

std::ostream& operator<<(std::ostream& os, const Token& token);
std::ostream& operator<<(std::ostream& os, const TokenKind& token_type);

struct JsonParser {
    static Json *parse(std::istream &istream);

private:
    static void tokenize(std::istream &istream);
    static Json *parse_json();
    static JsonValue *parse_json_value();

    static void expect_token(TokenKind token_type);
    static std::vector<Token> tokens;
    static size_t token_ptr;
    static const Token *current_token;
    static void next_token();
};

#endif //JSONPARSER_JSONPARSER_H
