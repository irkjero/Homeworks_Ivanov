//
// Created by Петр on 26.11.2020.
//

#ifndef HASHMAP_HASHMAP_H
#define HASHMAP_HASHMAP_H

#include "HashNode.h"
#include <iostream>
#include <limits>


template <class key, class T, class TAllocator>
class HashMap {
    using NodeType = HashNode<key, T, TAllocator>;
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
        NodeType* operator->() { return current_node; }
        bool operator==(const self_type& rhs) {
            return (nonEmptyBasket == rhs.nonEmptyBasket) && (current_node == rhs.current_node);
        }
        bool operator!=(const self_type& rhs) {
            return (nonEmptyBasket != rhs.nonEmptyBasket) || (current_node != rhs.current_node);
        }
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
        NodeType* current_node = nullptr;
        HashMap* map;
    };

    class const_iterator
    {
    public:
        typedef const_iterator self_type;

        const_iterator(HashMap const * map, unsigned long start_basket=0) : nonEmptyBasket{start_basket}, current_node{nullptr}, map{map} { }
        const_iterator& operator++() {
            rightNotZero();
            return *this;
        }
        T operator*() const {return current_node->getValue(); }
        NodeType* operator->() { return current_node; }
        bool operator==(const self_type& rhs) const {
            return (nonEmptyBasket == rhs.nonEmptyBasket) && (current_node == rhs.current_node);
        }
        bool operator!=(const self_type& rhs) const {
            return (nonEmptyBasket != rhs.nonEmptyBasket) || (current_node != rhs.current_node);
        }
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
        NodeType* current_node = nullptr;
        HashMap const * map;
    };

    HashMap() {
        // construct zero initialized hash table of size
        table = new NodeType* [TABLE_SIZE]();
        for(int i = 0; i < TABLE_SIZE; i++){
            table[i] = nullptr;
        }
    }


    HashMap(const std::initializer_list <std::pair<key, T> > init_list) : HashMap(){
        for (auto &element : init_list)
        {
            this->insert(element.first, element.second);
            size_++;
        }
    }


    HashMap(const HashMap<key, T, TAllocator>& other) :
        table{other.table}, TABLE_SIZE{other.TABLE_SIZE}, size_{other.size_}{
    }

    HashMap(HashMap<key, T, TAllocator>&& other) :
    table{other.table}, TABLE_SIZE{other.TABLE_SIZE}, size_{other.size_}{
        other.table = nullptr;
        other.TABLE_SIZE = 0;
        size_ = 0;
    }

    virtual ~HashMap(){
        // destroy all buckets one by one
        clear();
        // destroy the hash table
        delete [] table;
        size_ = 0;
    }

    HashMap& operator=(const HashMap<key, T, TAllocator>& right){
        if(this == &right)
            return *this;
        table = right.table;
        TABLE_SIZE = right.TABLE_SIZE;
        size_ = right.size_;
        return *this;
    }

    HashMap& operator=(HashMap<key, T, TAllocator>&& right){
        if(this == &right)
            return *this;

        HashMap tmp(std::forward<HashMap<key, T, TAllocator>>(right));
        std::swap(tmp.table, table);
        std::swap(tmp.TABLE_SIZE, TABLE_SIZE);
        std::swap(tmp.size_, size_);

        return *this;
    }

    template<class kk, class TT>
    bool insert(kk&& k, TT&& value){
        std::lock_guard<std::mutex> guard(mutex);
        unsigned long hashValue = get_hash(k);
        NodeType* entry = table[hashValue];
        if(entry == nullptr){
            auto* new_node = new HashNode<key, T, TAllocator>(k, value);
            table[hashValue] = new_node;
            return true;
        }

        while (entry != nullptr) {
            if (entry->getKey() == k) {
                entry->setValue(value);
                return false;
            }
            if (entry->getNext() == nullptr){
                auto* new_node = new HashNode<key, T, TAllocator>(k, value);
                entry->setNext(new_node);
            }
            entry = entry->getNext();
        }
        size_++;
        return true;
    }

    T get(key keyy, T defaul_value){
        std::lock_guard<std::mutex> guard(mutex);
        unsigned long hashValue = get_hash(keyy);
        NodeType* entry = table[hashValue];
        while (entry != nullptr) {
            if (entry->getKey() == keyy) {
                return entry->getValue();
            }
            entry = entry->getNext();
        }
        return defaul_value;
    }

    void exchange(key& keyy, T& value){
        std::lock_guard<std::mutex> guard(mute2);
        std::cout << keyy << "  " << value << std::endl;
        this->insert(keyy, value);
        this->get(keyy, value);
        std::cout << keyy << "  " << this->get(keyy, value) << std::endl << std::endl;

    }


    T& operator[](const key& keyy){
        std::lock_guard<std::mutex> guard(mutex);
        unsigned long hashValue = get_hash(keyy);
        NodeType* entry = table[hashValue];

        while (entry != nullptr) {
            if (entry->getKey() == keyy) {
                return entry->getValue();
            }
            entry = entry->getNext();
        }
        throw std::invalid_argument( "Key not found" );
    }

    T operator[](const key& keyy) const{
        std::lock_guard<std::mutex> guard(mutex);
        unsigned long hashValue = get_hash(keyy);
        NodeType* entry = table[hashValue];

        while (entry != nullptr) {
            if (entry->getKey() == keyy) {
                return entry->getValue();
            }
            entry = entry->getNext();
        }
        throw std::invalid_argument( "Key not found" );
    }

    T operator[](key&& keyy){
        std::lock_guard<std::mutex> guard(mutex);
        unsigned long hashValue = get_hash(keyy);
        NodeType* entry = table[hashValue];

        while (entry != nullptr) {
            if (entry->getKey() == keyy) {
                return entry->getValue();
            }
            entry = entry->getNext();
        }
        throw std::invalid_argument( "Key not found" );
    }


    iterator find(const key& keyy){
        std::lock_guard<std::mutex> guard(mutex);
        unsigned long hashValue = get_hash(keyy);
        NodeType* entry = table[hashValue];

        while (entry != nullptr) {
            if (entry->getKey() == keyy) {
                return entry->getValue();
            }
            entry = entry->getNext();
        }
        return end();
    }


    std::size_t size() const noexcept{
        return size_;
    }

    bool empty() const{
        std::lock_guard<std::mutex> guard(mutex);
        for(int i = 0; i < TABLE_SIZE; i++){
            if(table[i] != nullptr)
                return false;
        }
        return true;
    }

    void clear(){
        std::lock_guard<std::mutex> guard(mutex);
        for (int i = 0; i < TABLE_SIZE; ++i) {
            NodeType* entry = table[i];
            while (entry != nullptr) {
                NodeType* prev = entry;
                entry = entry->getNext();
                delete prev;
            }
            table[i] = nullptr;
        }
    }

    auto get_hash(key x) const {
        return hash(x) % TABLE_SIZE;
    }

    iterator begin(){
        return ++iterator(this, 0);
    }
    iterator end(){
        iterator it = iterator(this, TABLE_SIZE-1);
        return it;
    }
    const_iterator begin() const{
        return ++const_iterator(this, 0);
    }
    const_iterator end() const{
        const_iterator it = const_iterator(this, TABLE_SIZE-1);
        return it;
    }
    const_iterator cbegin() const {
        return ++const_iterator(this, 0);
    }
    const_iterator cend() const {
        const_iterator it = const_iterator(this, TABLE_SIZE-1);
        return it;
    }

private:
    // hash table

    NodeType **table;
    constexpr auto const static hash = std::hash<key>();
    unsigned long TABLE_SIZE = std::numeric_limits<unsigned short>::max();
    unsigned long size_;
    mutable std::mutex mutex;
    mutable std::mutex mute2;
};


#endif //HASHMAP_HASHMAP_H
