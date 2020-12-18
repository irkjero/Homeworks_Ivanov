//
// Created by Петр on 18.12.2020.
//

#ifndef TEST_18_12_20_PAINTER_H
#define TEST_18_12_20_PAINTER_H
#include "Car.h"

class Painter{
    Car car;
    std::recursive_mutex& mu;
public:
    Painter(Car car_, std::recursive_mutex &mu_): car{car_}, mu{mu_}{}

    bool paint(int a){
        //std::lock_guard<std::recursive_mutex> guard(mu);
        mu.lock();
        car.change_color(a);
    }
};
#endif //TEST_18_12_20_PAINTER_H
