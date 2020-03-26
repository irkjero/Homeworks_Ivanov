cmake_minimum_required(VERSION 3.15)
project(parser_csv)

set(CMAKE_CXX_STANDARD 17)

add_executable(parser_csv main.cpp CSVParser.h)