//
// Created by Петр on 18.12.2020.
//

#ifndef TEST_18_12_20_PAINTER_H
#define TEST_18_12_20_PAINTER_H
#include "Car.h"

class Painter{
    Car* car;
public:
    Painter(Car* car_){
        car = car_;
    }

    void paint(int a){
        mutex->lock();
        car->change_color(a);
    }
    std::mutex* mutex = nullptr;
};

#endif //TEST_18_12_20_PAINTER_H
