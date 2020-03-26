//
// Created by Петр on 26.03.2020.
//
#include <string>
#include <vector>
#include <iostream>
#include "InputIterator.h"

#ifndef PARSER_CSV_CSVPARSER_H
#define PARSER_CSV_CSVPARSER_H

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

    begin() {
        auto i =iterator;
        for (int32_t j = 0; j <= skipped_lines; ++j) {
            if (i.isEnd) {
                return ;
            }
            ++i;
        }
        return i;
    }

     end() {
        return ;
    }

};


#endif //PARSER_CSV_CSVPARSER_H
