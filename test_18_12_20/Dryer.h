//
// Created by Петр on 18.12.2020.
//

#ifndef TEST_18_12_20_DRYER_H
#define TEST_18_12_20_DRYER_H

#include "Car.h"

class Dryer{
    Car car;
    std::recursive_mutex& mu;
public:
    Dryer(Car car_, std::recursive_mutex &mu_): car{car_}, mu{mu_}{}

    bool dry(){
//        std::lock_guard<std::recursive_mutex> guard(mu);
        car.dry_car();
        mu.unlock();
    }
};

#endif //TEST_18_12_20_DRYER_H
