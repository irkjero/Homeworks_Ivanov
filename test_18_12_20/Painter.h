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
        std::unique_lock<std::mutex> lk{*mutex};
        cond->wait(lk, [=]{return *flag;});
        *flag = false;
        car->change_color(a);
    }
    std::mutex* mutex = nullptr;
    bool *flag = nullptr;
    std::condition_variable *cond = nullptr;
};

#endif //TEST_18_12_20_PAINTER_H
