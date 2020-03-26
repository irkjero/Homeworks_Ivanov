//
// Created by Петр on 26.03.2020.
//
#include <string>
#include <vector>
#include <iostream>

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

};


#endif //PARSER_CSV_CSVPARSER_H
