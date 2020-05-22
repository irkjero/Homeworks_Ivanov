#include <iostream>
#include <math.h>
#include <functional>
#include "Task2.h"
#include "Task3.h"
#include "Task4.h"
#include "Task5.h"


int main() {
    // Задача 2
    std::cout << std::endl;
    std::cout << "Task 2" << std::endl;
    try{
        std::cout << "3 == 5: " << close_enough(3,5) << std::endl;
        std::cout << "5 == 5: " << close_enough(5,5) << std::endl;

        std::cout << "4.9 == 5: " << close_enough(4.9,5.) << std::endl;
        std::cout << "5 == 5" << close_enough(5.,5.) << std::endl;
        std::cout << "4 == 5: " << close_enough(4.,5.) << std::endl;

        assert(close_enough(3, 5) == 0);
        assert(close_enough(5, 5) == 1);
        assert(close_enough(4.9, 5.) == 0);
        assert(close_enough(4. ,5.) == 0);
    }
    catch(char *str){
        std::cout << str << std::endl;
    }

    // Задача 3
    std::cout << std::endl;
    std::cout << "Task 3" << std::endl;
    std::cout << "Using 2 func: " << sum(1,2,3,4,5,6) << std::endl;
    std::cout << "Using fold: " << sum2(1,2,3,4,5) << std::endl;
    assert(sum2(1,2,3,4,5) == 15);
    assert(sum(1,2,3,4,5) == 15);
    assert(sum2(1) == 1);
    assert(sum(1) == 1);


    // Задача 4
    std::cout << std::endl;
    std::cout << "Task 4" << std::endl;
    auto plus = [](auto a, auto b) {return a + b;};
    auto minus = [](auto a, auto b) {return a - b;};
    auto mult = [](auto a, auto b) {return a*b;};
    std::cout << "Sum of 5: " << reduce(plus, 1,2,3,4,5) << std::endl;
    std::cout << "Mult of 5: " << reduce(mult, 1,2,3,4,5) << std::endl;
    std::cout << "Minus of 5: " << reduce(minus, 1,2,3,4,5) << std::endl;
    std::cout << "Sum of 5: " << reduce(plus, 5,4,3,2,1) << std::endl;
    std::cout << "mult of 1 and 0: " << reduce(mult, 1, 0) << std::endl;
    std::cout << "Minus of 5: " << reduce(minus, 5,4,3,2,1) << std::endl;
    assert(reduce(plus, 1,2,3,4,5) == 15);
    assert(reduce(mult, 1,2,3,4,5) == 120);
    assert(reduce(mult, 1, 0) == 0);
    assert(reduce(minus, 5,4,3,2,1) == 3);

    // Задача 5
    std::cout << std::endl;
    std::cout << "Task 5" << std::endl;
    using type_f1 = decltype(&ff1);
    using type_f2 = decltype(&ff2);
    using arg1 = function_traits<decltype(ff1)>::argument<0>::type;
    using arg2 = function_traits<decltype(ff1)>::argument<1>::type;
    using ret_type = function_traits<decltype(ff1)>::return_type;
    auto f_tmp = multiply_functions<type_f1, type_f2, ret_type, arg1, arg2>(ff1, ff2);

    std::cout << f_tmp(1.241231, 5.6) << std::endl;
    std::cout << f_tmp(123, 234234) << std::endl;
    assert(f_tmp(1.241231, 5.6) == 1);
    assert(f_tmp(324534, 31434) == 1);
    assert(f_tmp(1.2312314, 56) == 1);
    assert(f_tmp(1234213.3534, 1e-5) == 1);

    auto ff4 = ff33(ff1, ff2);
    std::cout << ff4(1,4) << std::endl;
    assert(ff4(1.2312314, 56) == 1);
    assert(ff4(1234213.3534, 1e-5) == 1);
    return 0;
}