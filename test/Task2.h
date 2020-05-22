//
// Created by Петр on 22.05.2020.
//
#ifndef TEST_TASK2_H
#define TEST_TASK2_H

template<typename T>
bool close_enough(T a, T b){
    if(std::is_integral<T>::value){
        return a == b;
    }
    else if (std::is_floating_point<T>::value){
        return fabs(a-b) < 1e-3;
    }
    else{
        throw "Not correct input" ;
    }
}

#endif //TEST_TASK2_H
