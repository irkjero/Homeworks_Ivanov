//
// Created by Петр on 22.05.2020.
//

#ifndef TEST_TASK5_H
#define TEST_TASK5_H
// Задача 5

// Через лямбду
auto ff33 = [] (auto f1, auto f2){ return [f1, f2](auto x, auto y){return f1(x, y) *f2(x, y);};};

template<class F>
struct function_traits;

template<class R, class... Args>
struct function_traits<R(*)(Args...)> : public function_traits<R(Args...)>
{};

template<class R, class... Args>
struct function_traits<R(Args...)>
{
    using return_type = R;

    template <std::size_t N>
    class argument
    {
    public:
        using type = typename std::tuple_element<N,std::tuple<Args...>>::type;
    };
};

template<typename A, typename RetType, typename Arg1, typename Arg2>
struct mult_f {
    mult_f(A ff1, A ff2) : f1{ff1}, f2{ff2} {}
    RetType operator()(Arg1 x, Arg2 y) { return f1(x, y) * f2(x, y); }

private:
    A f1;
    A f2;
};

template<class A, class B, class RetType, class Arg1, class Arg2>
auto multiply_functions(A f1, B f2){
    auto f3 = mult_f<A, RetType, Arg1, Arg2>(f1, f2);
    return f3;
}


auto ff1(double x, double y){
    return x*x - y*y;
}

auto ff2(double x, double y){
    return 1/((x*x) - (y*y));
}


#endif //TEST_TASK5_H
