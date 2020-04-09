//
// Created by Петр on 09.04.2020.
//

#ifndef INTEGTAL_FUNCBANK_H
#define INTEGTAL_FUNCBANK_H


struct square{
    constexpr static double my_function(double x) {
        return x * x ;
    }
};


struct cubic{
    constexpr static double my_function(double x) {
        return x * x * x ;//- 7 * (1 + x * x / 2 + x * x * x * x / 24);
    }
};


struct poly{
    constexpr static double my_function(double x) {
        return x * x * x + 2 * x * x + x ;//- 7 * (1 + x * x / 2 + x * x * x * x / 24);
    }
};

#endif //INTEGTAL_FUNCBANK_H
