//
// Created by Петр on 22.05.2020.
//

#ifndef TEST_TASK3_H
#define TEST_TASK3_H

template <class Arg>
Arg tmp_sum(Arg arg) { return arg; }

template <class First, class... Other>
auto tmp_sum(First first, Other... other)
{
    return first + tmp_sum(other...);
}

template<class ...Args>
auto sum(Args ...values){
    return tmp_sum(values...);
}

// Задача 3.2
template<class ...Args>
auto sum2(Args ...values){
    return (values + ...);
}

#endif //TEST_TASK3_H
