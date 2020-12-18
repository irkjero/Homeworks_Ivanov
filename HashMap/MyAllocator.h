//
// Created by Петр on 17.12.2020.
//
#include <iostream>

#ifndef HASHMAP_MYALLOCATOR_H
#define HASHMAP_MYALLOCATOR_H

template <typename T>
class MyAllocator : public std::allocator<T>{
private:
    std::size_t mem_usage;
    using Base = std::allocator<T>;
    using Pointer = typename std::allocator_traits<Base>::pointer;
    using Size = typename std::allocator_traits<Base>::size_type;

public:
    MyAllocator() = default;

    template <typename U>
    MyAllocator(const MyAllocator<U>& alloc) : Base(alloc){}

    Pointer allocate(Size b)
    {
        mem_usage = sizeof(T) * b;
        std::cout << "Alloc: " << mem_usage << std::endl;
        return Base::allocate(b);
    }

    template <class _Other>
    struct rebind {
        using other = MyAllocator<_Other>;
    };

    void deallocate(Pointer p, Size s)
    {
        std::cout << "Deallocate" << std::endl;
        Base::deallocate(p, s);
    }
};


#endif //HASHMAP_MYALLOCATOR_H
