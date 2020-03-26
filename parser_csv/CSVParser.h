//
// Created by Петр on 26.03.2020.
//

#ifndef PARSER_CSV_CSVPARSER_H
#define PARSER_CSV_CSVPARSER_H

#include <string>
#include <vector>
#include <iostream>
#include "InputIterator.h"

template<typename... Arguments>
class CSVParser {
public:
    std::ifstream *file;
    int32_t skipped_lines = 0;
    char separator = ',';

    CSVParser(std::ifstream *input_file, int skipped_line, char separator) {
        file = input_file;
        skipped_lines = skipped_line;
        this->separator = separator;
    }

    InputIterator<Arguments...> begin() {
        auto i =InputIterator<Arguments...>(file, false, separator);
        for (int32_t j = 0; j <= skipped_lines; ++j) {
            if (i.isEnd) {
                return InputIterator<Arguments...>(file, true, separator);
            }
            ++i;
        }
        return i;
    }

    InputIterator<Arguments...> end() {
        return InputIterator<Arguments...>(file, true, separator);;
    }

};


#endif //PARSER_CSV_CSVPARSER_H
