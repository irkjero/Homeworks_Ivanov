//
// Created by Петр on 18.12.2020.
//

#ifndef TEST_18_12_20_SPINLOCK_H
#define TEST_18_12_20_SPINLOCK_H


class SpinLock {
    std::atomic_flag locked = ATOMIC_FLAG_INIT ;
public:
    void lock() {
        while (locked.test_and_set(std::memory_order_acquire)) { ; }
    }
    void unlock() {
        locked.clear(std::memory_order_release);
    }
};


#endif //TEST_18_12_20_SPINLOCK_H
