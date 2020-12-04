//
// Created by Петр on 26.11.2020.
//

#ifndef HASHMAP_HASHMAP_H
#define HASHMAP_HASHMAP_H

#include <cstddef>
#include "HashNode.h"
#include <iterator>
#include <CoreFoundation/CoreFoundation.h>
#include <stdexcept>
#include <optional>
#include <iostream>


template <class key, class T, class TAllocator>
class HashMap {
    using node_type = HashNode<key, T>;
public:

    class iterator
    {
    public:
        typedef iterator self_type;

        iterator(HashMap* map, unsigned long start_basket=0) : nonEmptyBasket{start_basket}, current_node{nullptr}, map{map} { }
        iterator& operator++() {
            rightNotZero();
            return *this;
        }
        T operator*() {return current_node->getValue(); }
        node_type * operator->() { return current_node; }
        bool operator==(const self_type& rhs) { return (nonEmptyBasket == rhs.nonEmptyBasket) && (current_node == rhs.current_node); }
        bool operator!=(const self_type& rhs) { return (nonEmptyBasket != rhs.nonEmptyBasket) || (current_node != rhs.current_node); }
    private:
        void rightNotZero ()
        {
            if(nonEmptyBasket != 0){
                if(current_node->getNext() == nullptr){
                    ++nonEmptyBasket;
                    current_node = nullptr;
                }
            }
            for (unsigned long i = nonEmptyBasket; i < map->TABLE_SIZE; ++i)
            {
                if (map->table[i] != nullptr)
                {
                    nonEmptyBasket = i;
                    current_node = nullptr;
                    break;
                }
                if (i == map->TABLE_SIZE-1){
                    nonEmptyBasket = map->TABLE_SIZE-1;
                }
            }
            if(current_node == nullptr){
                current_node = map->table[nonEmptyBasket];
            }
            else{
                current_node = current_node->getNext();
            }
        }

        unsigned long nonEmptyBasket;
        HashNode<key, T>* current_node = nullptr;
        HashMap* map;
    };

    class const_iterator
    {
    public:
        typedef iterator self_type;

        const_iterator(HashMap* map, unsigned long start_basket=0) : nonEmptyBasket{start_basket}, current_node{nullptr}, map{map} { }
        iterator& operator++() {
            rightNotZero();
            return *this;
        }
        T operator*() {std::cout << current_node << std::endl; return current_node->getValue(); }
        node_type * operator->() { return current_node; }
        bool operator==(const self_type& rhs) { return (current_node == rhs.current_node); }
        bool operator!=(const self_type& rhs) { return (current_node != rhs.current_node); }
    private:
        void rightNotZero ()
        {
            if(nonEmptyBasket != 0){
                if(current_node->getNext() == nullptr){
                    ++nonEmptyBasket;
                    current_node = nullptr;
                }
            }

            for (unsigned long i = nonEmptyBasket; i < map->TABLE_SIZE; ++i)
            {
                if (map->table[i] != nullptr)
                {
                    nonEmptyBasket = i;
                    break;
                }
                nonEmptyBasket--;
            }
            std::cout << nonEmptyBasket << std::endl;

            if(current_node == nullptr){
                current_node = map->table[nonEmptyBasket];
            }
            else{
                current_node = current_node->getNext();
            }
        }

        unsigned long nonEmptyBasket;
        HashNode<key, T>* current_node = nullptr;
        HashMap* map;
    };

    HashMap() {
        // construct zero initialized hash table of size
        table = new node_type *[TABLE_SIZE]();
        for(int i = 0; i < TABLE_SIZE; i++){
            table[i] = nullptr;
        }
    }

    explicit HashMap(const TAllocator& alloc){

    }

    HashMap(std::initializer_list<T> il, const TAllocator& alloc){

    }

    HashMap(const HashMap<key, T, TAllocator>& other) :
    table{other.table}, TABLE_SIZE{other.TABLE_SIZE}{
        hash = other.hash;
    }

    HashMap(HashMap<key, T, TAllocator>&& other) :
    table{other.table}, TABLE_SIZE{other.TABLE_SIZE}{
        hash = other.hash;

        other.table = nullptr;
        other.TABLE_SIZE = 0;
    }

    virtual ~HashMap(){
        // destroy all buckets one by one
        clear();
        // destroy the hash table
        delete [] table;
    }

    HashMap& operator=(const HashMap<key, T, TAllocator>& right){
        if(this == &right)
            return *this;
        table = right.table;
        TABLE_SIZE = right.TABLE_SIZE;
        hash = right.hash;
        return *this;
    }

    HashMap& operator=(HashMap<key, T, TAllocator>&& right){
        if(this == &right)
            return *this;

        HashMap tmp(std::forward<HashMap<key, T, TAllocator>>(right));
        std::swap(tmp.table, table);
        std::swap(tmp.TABLE_SIZE, TABLE_SIZE);

        return *this;
    }


    template <class E, class P>
    bool insert(E&& k, P&& value){
        std::mutex insert_mutex;
        unsigned long hashValue = get_hash(k);
        HashNode<key, T> *entry = table[hashValue];

        if(entry == nullptr){
            auto* new_node = new HashNode<key, T>(k, value);
            insert_mutex.lock();
            table[hashValue] = new_node;
            insert_mutex.unlock();
            return true;
        }

        insert_mutex.lock();
        while (entry != nullptr) {
            if (entry->getKey() == k) {
                return false;
            }
            if (entry->getNext() == nullptr){
                auto* new_node = new HashNode<key, T>(k, value);
                entry->setNext(new_node);
            }
            entry = entry->getNext();
        }
        insert_mutex.unlock();
        return true;
    }

    T get(const key& keyy, T defaul_value){
        std::mutex get_mutex;
        unsigned long hashValue = get_hash(keyy);
        HashNode<key, T> *entry = table[hashValue];
        get_mutex.lock();
        while (entry != nullptr) {
            if (entry->getKey() == keyy) {
                return entry->getValue();
            }
            entry = entry->getNext();
        }
        get_mutex.unlock();
        return defaul_value;
    }


    T& operator[](const key& keyy){
        std::mutex get_mutex;
        unsigned long hashValue = get_hash(keyy);
        HashNode<key, T> *entry = table[hashValue];

        get_mutex.lock();
        while (entry != nullptr) {
            if (entry->getKey() == keyy) {
                return entry->getValue();
            }
            entry = entry->getNext();
        }
        get_mutex.unlock();
        throw std::invalid_argument( "Key not found" );
    }

    T operator[](key&& keyy){
        std::mutex get_mutex;
        unsigned long hashValue = get_hash(keyy);
        HashNode<key, T> *entry = table[hashValue];

        get_mutex.lock();
        while (entry != nullptr) {
            if (entry->getKey() == keyy) {
                return entry->getValue();
            }
            entry = entry->getNext();
        }
        get_mutex.unlock();
        throw std::invalid_argument( "Key not found" );
    }


    iterator find(const key& keyy){
        std::mutex get_mutex;
        unsigned long hashValue = get_hash(keyy);
        HashNode<key, T> *entry = table[hashValue];

        get_mutex.lock();
        while (entry != nullptr) {
            if (entry->getKey() == keyy) {
                return entry->getValue();
            }
            entry = entry->getNext();
        }
        get_mutex.unlock();
        return end();
    }


    std::size_t size() const noexcept{ return TABLE_SIZE;}

    bool empty() const noexcept{
        std::mutex empty_mutex;
        empty_mutex.lock();
        for(int i = 0; i < TABLE_SIZE; i++){
            if(table[i] != nullptr)
                return false;
        }
        empty_mutex.unlock();
        return true;
    }

    void clear(){
        std::mutex clear_mutex;
        clear_mutex.lock();
        for (int i = 0; i < TABLE_SIZE; ++i) {
            HashNode<key, T> *entry = table[i];
            while (entry != nullptr) {
                HashNode<key, T> *prev = entry;
                entry = entry->getNext();
                delete prev;
            }
            table[i] = nullptr;
        }
        clear_mutex.unlock();
    }

    auto get_hash(key x){
        return hash(x) % TABLE_SIZE;
    }

    iterator begin(){ return ++iterator(this, 0);}
    iterator end(){
        iterator it = iterator(this, TABLE_SIZE-1);
        return it;
    }
    const_iterator begin() const{return const_iterator(this, 0);}
    const_iterator end() const{ const_iterator it = const_iterator(this, TABLE_SIZE-1);}
    const_iterator cbegin() const {return const_iterator(this, 0);}
    const_iterator cend() const {const_iterator it = const_iterator(this, TABLE_SIZE-1);}

private:
    // hash table
    node_type **table;
    constexpr auto const static hash = std::hash<key>();
    unsigned long TABLE_SIZE = 30000;
};


#endif //HASHMAP_HASHMAP_H
