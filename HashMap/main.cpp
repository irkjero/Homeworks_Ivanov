#include <iostream>
#include "HashMap.h"
#include <vector>
#include <thread>
#include "MyAllocator.h"

//template<class M, class K, class V>
//auto get_smth(M& map, K key, V value){
//    for(int i = 0; i < 1000; i ++) {
//        map.insert(i, double(i + 0.1));
//    }
//    std::cout << map.get(key, value) << std::endl;
//    return map.get(key, value);
//}


template<class M, class K, class V>
void get_smth(M& map, K key, V value){
    std::cout << map.get(key, value) << std::endl;
}

template<class M, class K, class V>
void insert_smth(M& map, K key, V value){
    map.exchange(key, value);
}


int main() {
    // insert(r value and l value) +
    // чтобы работало с const map2[] +
    // initializer list +
    // allocator +

    using MyLovelyHash = HashMap<char, double, std::allocator<std::pair<char, double>>>;
    //using MyLovelyHash = HashMap<char, double, MyAllocator<std::pair<char, double>>>;
    MyLovelyHash map =MyLovelyHash();
    // INIT LIST
    MyLovelyHash const map2 {std::pair('w', 6.9), std::pair('d', 8.5)};
    std::cout << map2['w'] << std::endl; // const

    for(MyLovelyHash::const_iterator i = map2.begin(); i != map2.end(); ++i){
        std::cout << "Value2: " << *i << std::endl;
    }

    for(auto i : map2){
        std::cout << "Value2: " << i << std::endl;
    }

    //map = map2;
    //map = std::move(map2);

    for(auto i : map){
        std::cout << "Value1: " << i << std::endl;
    }

    std::cout << map.empty() << std::endl;
    std::cout << map2.empty() << std::endl;

    char rr = 'r';
    map.insert(rr, 6.87);
    map.insert('p', 3.14);
    map.insert('e', 2.71);

    std::cout << map['p'] << std::endl;
    std::cout << map['e'] << std::endl;
    std::cout << map.get('p', 3.15) << std::endl;
    std::cout << map.get('g', 9.8) << std::endl;
    std::cout << map.empty() << std::endl;

    std::cout << map.insert('p', 10.14);

    for(MyLovelyHash::iterator i = map.begin(); i != map.end(); ++i){
        std::cout << "Value: " << *i << std::endl;
    }

    for(double i : map){
        std::cout << "Value: " << i << std::endl;
    }

    std::cout << "Hello, World!" << std::endl;

    for(int i = 0; i < 101; i ++)
        map.insert(char(i), i+0.1);

    std::cout << map[char(100)] << std::endl;
    std::cout << map.get(char(100), 0) << std::endl;


    using MyLovelyHash2 = HashMap<int, double, std::allocator<std::pair<int, double>>>;

    MyLovelyHash2 map_concurrent = MyLovelyHash2();
    map_concurrent.insert(10, 10.1);
    for(int i = 0; i < 1000; i ++) {
        map_concurrent.insert(i, double(i + 0.1));
    }

    std::thread t1(get_smth<MyLovelyHash2, int, double>, std::ref(map_concurrent), 875, -100500);
    std::thread t2(get_smth<MyLovelyHash2, int, double>, std::ref(map_concurrent), 875, -100500);
    std::thread t3(get_smth<MyLovelyHash2, int, double>, std::ref(map_concurrent), 875, -100500);
    std::thread t7(insert_smth<MyLovelyHash2, int, double>, std::ref(map_concurrent), 1002, 10101.1);
    std::thread t8(get_smth<MyLovelyHash2, int, double>, std::ref(map_concurrent), 1002, -100500);
    std::thread t4(get_smth<MyLovelyHash2, int, double>, std::ref(map_concurrent), 875, -100500);
    std::thread t5(get_smth<MyLovelyHash2, int, double>, std::ref(map_concurrent), 875, -100500);
    std::thread t6(get_smth<MyLovelyHash2, int, double>, std::ref(map_concurrent), 875, -100500);

    t1.join();
    t2.join();
    t3.join();
    t4.join();
    t5.join();
    t6.join();
    t7.join();
    t8.join();



    std::vector<std::thread > thread_pool;
    thread_pool.reserve(1000);
    void* someData;

    for(int i = 0; i < 1000; i++)
    {
        thread_pool.push_back(std::thread(insert_smth<MyLovelyHash2 , int, double>, std::ref(map_concurrent), 10, i));
    }
    for(int i = 0; i < 1000; i++){
        thread_pool[i].join();
    }


    return 0;
}