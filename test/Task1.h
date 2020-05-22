//
// Created by Петр on 22.05.2020.
//

#ifndef TEST_TASK1_H
#define TEST_TASK1_H

// Задача 1
template<class T>
class WeakPtr{
    T* sptr = nullptr;
public:
    WeakPtr(T* ptr) : sptr{ptr} {};
    ~WeakPtr(){
        delete sptr;
    };

    WeakPtr& operator=(const WeakPtr& that);
    bool operator==(const WeakPtr& that) const;

};

#endif //TEST_TASK1_H
