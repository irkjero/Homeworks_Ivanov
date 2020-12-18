//
// Created by Петр on 18.12.2020.
//

#ifndef TEST_18_12_20_CAR_H
#define TEST_18_12_20_CAR_H

class Car {
    bool is_dry;
    int color;
public:
    Car(bool is_dry_, int color_) : is_dry{is_dry_}, color{color_}{}

    void change_color(int new_color){
        std::cout << "paint" << std::endl;
        if(is_dry == false){
            std::cout << "We cant paint it" << std::endl;
        }
        else{
            color = new_color;
            is_dry = false;
        }
    }

    void dry_car(){
        is_dry = true;

    }

    int colour(){
        return color;
    }

};

#endif //TEST_18_12_20_CAR_H
