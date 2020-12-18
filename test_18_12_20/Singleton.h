//
// Created by Петр on 18.12.2020.
//

#ifndef TEST_18_12_20_SINGLETON_H
#define TEST_18_12_20_SINGLETON_H

#include <atomic>

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

class SingletonBarrier
{
    static std::atomic<SingletonBarrier *> s;
    SingletonBarrier() = default;
    virtual ~SingletonBarrier() {}
    static std::mutex myMutex;
public:
    SingletonBarrier* get(){
        std::atomic_thred_fence(std::memory_order_release);
        if(s.load(std::memory_order_acquire) == nullptr){
            s.store(new SingletonBarrier(), std::memory_order_release);
        }
        return s;
    }
};
std::atomic<SingletonBarrier *> SingletonBarrier::s = nullptr;

#endif //TEST_18_12_20_SINGLETON_H
