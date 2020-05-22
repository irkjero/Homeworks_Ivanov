//
// Created by Петр on 22.05.2020.
//

#ifndef TEST_TASK4_H
#define TEST_TASK4_H
// Задача 4
template <class Op, class Arg>
Arg reduce_tmp(Op f, Arg arg) { return arg; }

template <class Op, class First, class... Other>
auto reduce_tmp(Op f, First first, Other... other)
{
    return f(first, reduce_tmp(f, other...));
}

template<class Op, class ... Args>
auto reduce(Op f, Args ...values ){
    return reduce_tmp(f, values...);
}

#endif //TEST_TASK4_H
