//
// Created by Петр on 26.03.2020.
//

#ifndef PARSER_CSV_INPUTITERATOR_H
#define PARSER_CSV_INPUTITERATOR_H

#include <vector>
#include <memory>
#include <istream>
#include <fstream>

#include <iostream>


template<typename ... Args>
class InputIterator {
    std::ifstream *file;
public:
    bool isEnd = false;
    bool lastElement = false;
    std::tuple<Args...> lines;
    int32_t index = 0;
    char separator = ',';


    InputIterator(std::ifstream *input, bool isEnd, char separator) {
        file = input;
        this->isEnd = isEnd;
        this->separator = separator;
    }

    InputIterator &operator=(const InputIterator &rhs) {
        return (*this);
    }

    bool operator==(InputIterator rhs) {
        return (rhs.isEnd == this->isEnd);
    }

    bool operator!=(InputIterator rhs) {
        return rhs.isEnd != this->isEnd;
    }

    auto operator*() {
        return lines;
    }




}

#endif //PARSER_CSV_INPUTITERATOR_H
