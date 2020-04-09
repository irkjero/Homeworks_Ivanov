#include <iostream>
#include "Gausses.h"
#include "FuncBank.h"
#include "Integrate.h"

//template<typename argument>
//constexpr argument my_function(argument x) {
//    return x * x ;//- 7 * (1 + x * x / 2 + x * x * x * x / 24);
//}
//
//
//template<typename Function>
//constexpr double Integrate(Function my_function) {
//    constexpr int32_t leftNode = -5;
//    constexpr int32_t rightNode = 5;
//
//    return (rightNode - leftNode) / 6.0 * (my_function(leftNode) + 4 * my_function((leftNode + rightNode) / 2.0) +
//                                           my_function(rightNode));
//
//}


int main() {
//    constexpr auto f = my_function<double>;
//    std::cout << Integrate(f) <<std::endl;
//    static_assert(Integrate(f)-83.333 < 1e-3 and Integrate(f)-83.333 >= 0, "good");

    std::cout << Integrate<square, Order33>() << std::endl;
    static_assert(Integrate<square, Order33>() - 0.6666 < 1e-4 and Integrate<square, Order33>() - 0.6666 >= 0);

    std::cout << Integrate<square, Order55>() << std::endl;
    static_assert(Integrate<square, Order55>() - 0.6666 < 1e-4 and Integrate<square, Order55>() - 0.6666 >= 0);

    return 0;
}

