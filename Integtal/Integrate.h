//
// Created by Петр on 09.04.2020.
//

#ifndef INTEGTAL_INTEGRATE_H
#define INTEGTAL_INTEGRATE_H

#include <tuple>


template<typename Function , typename ord, int current, int order>
struct IntegrateRecurs{
    constexpr static double calculate() {
        return Function::my_function((std::get<current>(ord::points)))*std::get<current>(ord::weights) + IntegrateRecurs<Function, ord, current+1, order>::calculate();
    }
};


template<typename Function , typename ord, int order>
struct IntegrateRecurs<Function, ord, order, order>{
    constexpr static double calculate() {
        return 0;
    }
};


template<typename Function , typename ord>
constexpr double Integrate() {
    return IntegrateRecurs<Function, ord, 0, ord::order>::calculate();
}

#endif //INTEGTAL_INTEGRATE_H
