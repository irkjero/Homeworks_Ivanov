//
// Created by Петр on 26.11.2020.
//
#include <iostream>
#ifndef HASHMAP_HASHNODE_H
#define HASHMAP_HASHNODE_H


template <class K, class V, class TAllocator>
class HashNode {
public:
    HashNode(const K &key, const V &value) : next(nullptr) {
        data = AllocatorTraits::allocate(allocator, 1);
        AllocatorTraits::construct(allocator, data, std::pair<K, V>(key, value));
    }

    K getKey() const {
        return data->first;
    }

    V getValue() const {
        return data->second;
    }

    void setValue(V value) {
        HashNode::data->second = value;
    }

    HashNode *getNext() const {
        return next;
    }

    void setNext(HashNode *next) {
        HashNode::next = next;
    }

    ~HashNode(){
        AllocatorTraits::destroy(allocator, data);
        AllocatorTraits::deallocate(allocator, data, 1);
    }

private:
    // key-value pair
    using AllocatorTraits = std::allocator_traits<TAllocator>;
    std::pair<K, V> * data;
    TAllocator allocator;
    //K key;
    // value;
    // next bucket with the same key
    HashNode *next;
};


#endif //HASHMAP_HASHNODE_H
