#include <iostream>
#include "Stream.h"
#include "StreamTerminators.h"
#include <vector>

int main() {
    Stream<int> a = {3, 2, 3, 4, 5};
    Stream<int> a3 = {3, 2, 3, 4, 5, 11, 12, 13};
//    std::vector<double> vector1 {1.1, 2.2, 3.3};
//    Stream<double> a44(vector1);

//    auto sumed = a44 | sum();
//    std::cout << sumed << std::endl;
    auto ss = a | filter([=](const auto& x) { return x%2; }) | map_([=](const auto& x) { return x*x*x; });
    ss | print_to(std::cout, ",");
    std::cout << std::endl;

    auto third = a3 | filter([=](const auto& x) { return x%2; }) | map_([=](const auto& x) { return x*x*x; }) | nth(4);
    std::cout << third << std::endl;

    return 0;
}