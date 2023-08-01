//
// Created by isura on 1/08/2023.
//

#include <sstream>
#include <fstream>
#include <iostream>
#include <cassert>
#include <vector>

#include "JsonParser.h"


Token::Token(TokenKind kind) : kind(kind) {
}

std::ostream& operator<<(std::ostream& os, const Token& token) {
    switch(token.kind) {
        case TokenKind::LeftCurly:
            os << "[LeftCurly]";
            break;
        case TokenKind::RightCurly:
            os << "[RightCurly]";
            break;
        case TokenKind::LeftSquare:
            os << "[LeftSquare]";
            break;
        case TokenKind::RightSquare:
            os << "[RightSquare]";
            break;
        case TokenKind::Colon:
            os << "[Colon]";
            break;
        case TokenKind::Comma:
            os << "[Comma]";
            break;
        case TokenKind::String:
            os << "[String: " << std::get<std::string>(token.value) << "]";
            break;
        case TokenKind::Integer:
            os << "[Integer: " << std::get<int64_t>(token.value) << "]";
            break;
        case TokenKind::Float:
            os << "[Float: " << std::get<double>(token.value) << "]";
            break;
        case TokenKind::Boolean:
            os << "[Boolean: " << std::boolalpha << std::get<bool>(token.value) << "]";
            break;
        case TokenKind::Null:
            os << "[Null]";
            break;
    }
    return os;
}

std::ostream& operator<<(std::ostream& os, const TokenKind& token_type) {
    switch(token_type) {
        case TokenKind::LeftCurly:
            os << "[LeftCurly]";
            break;
        case TokenKind::RightCurly:
            os << "[RightCurly]";
            break;
        case TokenKind::LeftSquare:
            os << "[LeftSquare]";
            break;
        case TokenKind::RightSquare:
            os << "[RightSquare]";
            break;
        case TokenKind::Colon:
            os << "[Colon]";
            break;
        case TokenKind::Comma:
            os << "[Comma]";
            break;
        case TokenKind::String:
            os << "[String]";
            break;
        case TokenKind::Integer:
            os << "[Integer]";
            break;
        case TokenKind::Float:
            os << "[Float]";
            break;
        case TokenKind::Boolean:
            os << "[Boolean]";
            break;
        case TokenKind::Null:
            os << "[Null]";
            break;
    }
    return os;
}

std::vector<Token> JsonParser::tokens;
size_t JsonParser::token_ptr = 0;
const Token *JsonParser::current_token = nullptr;

Json *JsonParser::parse_file(const std::string &path) {
    std::ifstream istream(path);
    return parse_stream(istream);
}

Json *JsonParser::parse_string(const std::string &string) {
    std::istringstream istream(string);
    return parse_stream(istream);
}

Json *JsonParser::parse_stream(std::istream &istream) {
    JsonParser::tokens.clear();
    JsonParser::token_ptr = 0;
    JsonParser::current_token = nullptr;

    tokenize(istream);
    auto *json = parse_json();
    for(const auto &token: tokens) {
        std::cout << token << ' ';
    }
    std::cout << '\n';
    return json;
}

void JsonParser::tokenize(std::istream &istream) {
    size_t line_n = 0;
    size_t col_n = 0;
    char c;
    auto next_token = [&]() { c = static_cast<char>(istream.get()); ++col_n; };
    next_token();
    while(!istream.eof()) {
        switch(c) {
            case '\n':
                ++line_n;
                col_n = 0;
                next_token();
                break;
            case ' ': case '\t': case '\r': case '\v': case '\b': case '\f': case '\a':
                next_token();
                break;
            case '{':
                tokens.emplace_back(TokenKind::LeftCurly);
                next_token();
                break;
            case '}':
                tokens.emplace_back(TokenKind::RightCurly);
                next_token();
                break;
            case '[':
                tokens.emplace_back(TokenKind::LeftSquare);
                next_token();
                break;
            case ']':
                tokens.emplace_back(TokenKind::RightSquare);
                next_token();
                break;
            case ':':
                tokens.emplace_back(TokenKind::Colon);
                next_token();
                break;
            case ',':
                tokens.emplace_back(TokenKind::Comma);
                next_token();
                break;
            case '"': {
                Token token(TokenKind::String);
                std::ostringstream oss;
                do {
                    next_token();
                    switch (c) {
                        case '\\':
                            next_token();
                            switch (c) {
                                case 'n':
                                    oss << '\n';
                                    break;
                                case '"':
                                    oss << '"';
                                    break;
                                default:
                                    assert(0 && "Unidentified special character sequence");
                            }
                        case '\n':
                            assert(0 && "New lines are not allowed inside strings");
                        case '"':
                            break;
                        default:
                            oss << c;
                    }
                } while (c != '"');
                token.value = oss.str();
                tokens.push_back(token);
                next_token();
                break;
            }
            case '1': case '2': case '3': case '4': case '5': case '6': case '7': case '8': case '9': {  // Omit '0'
                long integer_value = 0;
                double float_value = 0;
                bool is_float = false;
                double ten_power = 0.1;
                while(c == '.' || (c >= '0' && c <= '9')) {
                    if(c == '.') {
                        float_value = integer_value;
                        is_float = true;
                    } else {
                        if(is_float) {
                            float_value = float_value + ten_power * (c - '0');
                            ten_power /= 10;
                        } else {
                            integer_value = integer_value * 10 + (c - '0');
                        }
                    }
                    next_token();
                }
                if(is_float) {
                    Token token(TokenKind::Float);
                    token.value = float_value;
                    tokens.emplace_back(token);
                } else {
                    Token token(TokenKind::Integer);
                    token.value = integer_value;
                    tokens.emplace_back(token);
                }
                break;
            }
            case 'a': case 'b': case 'c': case 'd': case 'e': case 'f': case 'g': case 'h': case 'i': case 'j':
            case 'k': case 'l': case 'm': case 'n': case 'o': case 'p': case 'q': case 'r': case 's': case 't':
            case 'u': case 'v': case 'w': case 'x': case 'y': case 'z':
            case 'A': case 'B': case 'C': case 'D': case 'E': case 'F': case 'G': case 'H': case 'I': case 'J':
            case 'K': case 'L': case 'M': case 'N': case 'O': case 'P': case 'Q': case 'R': case 'S': case 'T':
            case 'U': case 'V': case 'W': case 'X': case 'Y': case 'Z': {
                std::ostringstream oss;
                while(isalpha(c)) {
                    oss << c;
                    next_token();
                }
                std::string keyword = oss.str();
                if(keyword == "true") {
                    Token token(TokenKind::Boolean);
                    token.value = true;
                    tokens.push_back(token);
                } else if(keyword == "false") {
                    Token token(TokenKind::Boolean);
                    token.value = false;
                    tokens.push_back(token);
                } else if(keyword == "null") {
                    tokens.emplace_back(TokenKind::Null);
                } else {
                    assert(0 && "Unidentified keyword");
                }
                break;
            }
            default:
                std::cout << c << '\n';
                assert(0);
        }
    }
    current_token = &tokens[0];
}

Json *JsonParser::parse_json() {
    if(current_token->kind == TokenKind::LeftCurly) {
        auto *json = new Json(JsonKind::Map);
        json->object = std::unordered_map<std::string, JsonValue *>();
        while(true) {
            next_token();
            expect_token(TokenKind::String);
            const auto &key = std::get<std::string>(current_token->value);
            next_token();
            expect_token(TokenKind::Colon);
            next_token();
            auto *json_value = JsonParser::parse_json_value();
            auto &map = std::get<std::unordered_map<std::string, JsonValue *>>(json->object);
            if(map.contains(key)) {
                assert(0 && "Map already contains key");
            }
            map.emplace(key, json_value);
            next_token();
            if(current_token->kind != TokenKind::Comma) {
                break;
            }
        }
        expect_token(TokenKind::RightCurly);
        return json;
    } else if(current_token->kind == TokenKind::LeftSquare) {
        auto *json = new Json(JsonKind::Array);
        json->object = std::vector<JsonValue *>();
        while(true) {
            next_token();
            auto *json_value = JsonParser::parse_json_value();
            std::get<std::vector<JsonValue *>>(json->object).push_back(json_value);
            next_token();
            if(current_token->kind != TokenKind::Comma) {
                break;
            }
        }
        expect_token(TokenKind::RightSquare);
        return json;
    } else {
        assert(0 && "Expected token [ or {");
    }
}

JsonValue *JsonParser::parse_json_value() {
    switch(current_token->kind) {
        case TokenKind::Integer:
            return new JsonValue(JsonValueKind::Integer, std::get<int64_t>(current_token->value));
        case TokenKind::Float:
            return new JsonValue(JsonValueKind::Float, std::get<double>(current_token->value));
        case TokenKind::Boolean:
            return new JsonValue(JsonValueKind::Boolean, std::get<bool>(current_token->value));
        case TokenKind::String:
            return new JsonValue(JsonValueKind::String, std::get<std::string>(current_token->value));
        case TokenKind::Null:
            return new JsonValue(JsonValueKind::Null, nullptr);
        case TokenKind::LeftSquare:
            return new JsonValue(JsonValueKind::Array, std::get<std::vector<JsonValue *>>(JsonParser::parse_json()->object));
        case TokenKind::LeftCurly:
            return new JsonValue(JsonValueKind::Map, std::get<std::unordered_map<std::string, JsonValue *>>(JsonParser::parse_json()->object));
        default:
            assert(0 && "Unexpected token");
    }
}


void JsonParser::next_token() {
    current_token = &tokens[++JsonParser::token_ptr];
}


void JsonParser::expect_token(TokenKind token_type) {
    if(!current_token) {
        std::cout << "Null token\n";
        std::exit(1);
    }
    if(current_token->kind != token_type) {
        std::cout << "Expected token " << token_type << ", received " << current_token->kind << '\n';
        std::exit(1);
    }
}
