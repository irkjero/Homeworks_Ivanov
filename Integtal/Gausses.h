//
// Created by Петр on 09.04.2020.
//

#ifndef INTEGTAL_GAUSSES_H
#define INTEGTAL_GAUSSES_H

#include <tuple>

struct Order33{
    constexpr static std::tuple points = {0.0, 0.77459, -0.77459};
    constexpr static std::tuple weights =  {8.0/9.0,  5.0/9.0, 5.0/9.0};
    constexpr static int order = 3;
};


struct Order55{
    constexpr static std::tuple points = {0.0, 0.538469, -0.538469, 0.9061798, -0.9061798};
    constexpr static std::tuple weights =  {0.568889,  0.478629, 0.478629, 0.236927, 0.236927};
    constexpr static int order = 5;
};

struct gauss{
    static constexpr auto points = std::make_tuple(0.0, 0.538469, -0.538469, 0.9061798, -0.9061798);
    static constexpr auto weights =  std::make_tuple(0.568889,  0.478629, 0.478629, 0.236927, 0.236927);
};

#endif //INTEGTAL_GAUSSES_H
