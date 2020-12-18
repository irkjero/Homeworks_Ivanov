//
// Created by Петр on 18.12.2020.
//

#ifndef TEST_18_12_20_SPINLOCK_H
#define TEST_18_12_20_SPINLOCK_H


class SpinLock {
std::atomic<bool> is_lock = {false};

void lock() {
    while(is_lock.exchange(true, std::memory_order_acquire)) {;}
}

void unlock() {
    is_lock.store(false, std::memory_order_release); }
};

#endif //TEST_18_12_20_SPINLOCK_H
