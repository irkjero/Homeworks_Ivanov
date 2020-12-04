#include <iostream>
#include "HashMap.h"

int main() {
    HashMap<char, double, double> map = HashMap<char, double, double>();
    std::cout << map.empty() << std::endl;
    map.insert<char, double>('p', 3.14);
    map.insert<char, double>('e', 2.71);
    std::cout << map['p'] << std::endl;
    std::cout << map['e'] << std::endl;
    std::cout << map.get('p', 3.15) << std::endl;
    std::cout << map.get('g', 9.8) << std::endl;
    std::cout << map.empty() << std::endl;

    std::cout << map.insert<char, double>('p', 3.14);

    for(HashMap<char, double, double>::iterator i = map.begin(); i != map.end(); ++i){
        std::cout << "Value: " << *i << std::endl;
    }

    std::cout << "Hello, World!" << std::endl;
    return 0;
}