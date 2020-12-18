//
// Created by Петр on 18.12.2020.
//

#ifndef TEST_18_12_20_DRYER_H
#define TEST_18_12_20_DRYER_H

#include "Car.h"

class Dryer{
    Car* car;
public:
    Dryer(Car* car_){
        car = car_;
    }

    void dry(){
        car->dry_car();
      mutex->unlock();
    }
    std::mutex* mutex = nullptr;
};

#endif //TEST_18_12_20_DRYER_H
