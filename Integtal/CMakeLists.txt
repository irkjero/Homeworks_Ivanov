cmake_minimum_required(VERSION 3.15)
project(Integtal)

set(CMAKE_CXX_STANDARD 17)

add_executable(Integtal main.cpp Gausses.h Integrate.h FuncBank.h)