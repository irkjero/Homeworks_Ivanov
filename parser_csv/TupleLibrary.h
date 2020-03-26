//
// Created by Петр on 26.03.2020.
//

#ifndef PARSER_CSV_TUPLELIBRARY_H
#define PARSER_CSV_TUPLELIBRARY_H
#include <tuple>
#include <sstream>
#include <iostream>

template<class TupleType, int current, int size>
struct tuple_printer {
    static void print(const TupleType &tuple) {
        std::cout << std::get<current>(tuple) << ' ';
        tuple_printer<TupleType, current + 1, size>::print(tuple);
    }
};

template<class TupleType, int size>
struct tuple_printer<TupleType, size, size> {
    static void print(const TupleType &tuple) {
        std::cout << std::get<size>(tuple) << std::endl;
    }
};

template<class... Args>
void print_tuple(const std::tuple<Args...> &tuple) {
    tuple_printer<std::tuple<Args...>, 0, sizeof...(Args) - 1>::print(tuple);
}


template<typename T>
std::tuple<T> parse(std::istringstream *is, char separator, int line_number, int position) {
    T t;
    std::string s;
    std::string s1;
    std::getline(*is, s, separator);
    std::istringstream temp(s);
    temp >> t;

    if (!is->eof()) {
        std::string exception = "Some extra columns at line number"
                                +std::to_string(line_number + 1) + " pos number"+
                                std::to_string(position);
        throw std::runtime_error(exception);
    }

    if (is->fail()) {
        std::string exception = "Invalid format of input data! The error is at line number "
                                + std::to_string(line_number + 1) + "pos number" + std::to_string(position);
        throw std::runtime_error(exception);
    }

    return std::tuple<T>(std::move(t));
}

template<typename T, typename Arg, typename... Args>
std::tuple<T, Arg, Args...> parse(std::istringstream *is, char separator, int line_number, int position) {
    T t;
    std::string s;
    std::string s1;
    std::getline(*is, s, separator);
    std::istringstream temp(s);
    temp >> t;
//    position = is->tellg();

    std::cout << "pos: " << position << std::endl;
    std::cout << temp.fail() << is->eof() << std::endl;
    if (temp.fail() || is->eof()) {
        std::string exception = "Invalid format of input data! The error is at string numberssss "
                                + std::to_string(line_number + 1) + " pos number" + std::to_string(position);
        throw std::runtime_error(exception);
    }
    position = static_cast<int>(is->tellg()) + 1;
    return std::tuple_cat(std::tuple<T>(std::move(t)),
                          parse<Arg, Args...>(is, separator, line_number, position));

}

#endif //PARSER_CSV_TUPLELIBRARY_H
