#include <iostream>
#include <thread>
#include "MyThread.cpp"
#include "Dryer.h"
#include "Painter.h"
#include "Car.h"

void func(int i){
    std::cout << "Hello : " << i << std::endl;
}

void paint(Painter& p, int c){
    p.paint(c);
}

void dry(Dryer& d){
    d.dry();
}

std::recursive_mutex mu;

int main() {
    std::thread th11(func, 1);
    MyRAIIThread a2(th11);

    Car car1(true, 1);
    Dryer dryer(car1, mu);
    Painter painter1(car1, mu);
    mu.unlock();
    std::thread paint_th(paint, std::ref(painter1), 2);
    std::thread dry_th(dry, std::ref(dryer));
    std::thread paint_th2(paint, std::ref(painter1), 2);
    std::thread dry_th2(dry, std::ref(dryer));
    std::thread paint_th3(paint, std::ref(painter1), 2);
    std::thread dry_th3(dry, std::ref(dryer));

    paint_th.join();
    dry_th.join();
    paint_th2.join();
    dry_th2.join();
    paint_th3.join();
    dry_th3.join();

}
