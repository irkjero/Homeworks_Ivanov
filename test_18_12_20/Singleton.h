//
// Created by Петр on 18.12.2020.
//

#ifndef TEST_18_12_20_SINGLETON_H
#define TEST_18_12_20_SINGLETON_H

class Singleton
{
    static Singleton *s;
    Singleton() = default;
    virtual ~Singleton() {}
    static std::mutex myMutex;
public:
    Singleton* get(){
        if(s == nullptr){
            std::lock_guard<std::mutex> guard(myMutex);
            if(s == nullptr)
                s = new Singleton();
        }
        return s;
    }
};
Singleton * Singleton::s = nullptr;

#endif //TEST_18_12_20_SINGLETON_H
