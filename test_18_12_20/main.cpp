#include <iostream>
#include <thread>
#include "MyThread.cpp"


void func(int i){
    std::cout << "Hello : " << i << std::endl;
}

int main() {
    std::thread th11(func, 1);
    MyRAIIThread a2(th11);
}
