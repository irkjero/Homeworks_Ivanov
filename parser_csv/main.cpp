#include <iostream>
#include <fstream>
#include "CSVParser.h"
#include "TupleLibrary.h"

int main() {
    std::ifstream file("/Users/xloride/Desktop/Homeworks/Homeworks_c++/parser_csv/5_a16w16.csv");
    std::string line;
    auto sep = ' ';

    CSVParser<std::string, int, double, double, double, double, double> parser(&file, 0, sep);

    try {
        for (auto i = parser.begin(); i != parser.end(); ++i) {
            print_tuple(*i);
        }
    }
    catch (std::exception &error) {
        std::cout << error.what();
    }
    return 0;
}