cmake_minimum_required(VERSION 3.22)
project(JsonParser)

set(CMAKE_CXX_STANDARD 23)

add_executable(JsonParser main.cpp JsonParser.cpp JsonParser.h Json.cpp Json.h
        JsonDumper.cpp
        JsonDumper.h
)
