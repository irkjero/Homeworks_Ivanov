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
        std::unique_lock<std::mutex> lk{*mutex};
        cond->wait(lk, [=]{return !*flag;});
        std::cout << "dry" << std::endl;
        car->dry_car();
        *flag = true;
        cond->notify_one();
    }
    std::mutex* mutex = nullptr;
    bool *flag = nullptr;
    std::condition_variable *cond = nullptr;

};

#endif //TEST_18_12_20_DRYER_H
