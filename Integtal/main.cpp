#include <iostream>
#include "Gausses.h"
#include "FuncBank.h"
#include "Integrate.h"

//template<typename argument>
//constexpr argument my_function(argument x) {
//    return x * x ;//- 7 * (1 + x * x / 2 + x * x * x * x / 24);
//}
//
//
//template<typename Function>
//constexpr double Integrate(Function my_function) {
//    constexpr int32_t leftNode = -5;
//    constexpr int32_t rightNode = 5;
//
//    return (rightNode - leftNode) / 6.0 * (my_function(leftNode) + 4 * my_function((leftNode + rightNode) / 2.0) +
//                                           my_function(rightNode));
//
//}


template<typename T>
struct get_size
{
    template<typename Func, typename T1, typename T2>
    constexpr static auto get_elems(Func func, T1& w,  T2& p){
        return func(std::get<0>(p))*std::get<0>(w) ;
    }
};

template<typename T, typename ...Ts>
struct get_size<std::tuple<T, Ts...>>
{
    template<typename Func, typename T1, typename T2>
    constexpr static auto get_elems(Func func, T1& w, T2& p){
        constexpr int size = sizeof...(Ts);
        return func(std::get<size>(p))*std::get<size>(w) + get_size< std::tuple<Ts...> >::get_elems(func, w, p);//get_elems<Func,T1, T2>(func, w, p);
    }
};
//
template<typename T>
constexpr auto sizee(T tuple){
    return std::tuple_size<T>::value;
}

template<typename Func, typename T>
constexpr auto call(Func func, T weights, T points){
    return get_size<T>::get_elems(func, weights, points);
}

template<typename Function, typename ord>
constexpr auto integrate_2(Function func, ord int_w_p){
    auto res = call<Function>(func, int_w_p.weights, int_w_p.points);
    return res;
}


int main() {
//    constexpr auto f = my_function<double>;
//    std::cout << Integrate(f) <<std::endl;
//    static_assert(Integrate(f)-83.333 < 1e-3 and Integrate(f)-83.333 >= 0, "good");

    auto f = [](std::tuple<double> arg){ return std::get<0>(arg);};
    auto f_2 = [](double x){ return x*x;};

    constexpr auto res = integrate_2(f_2, gauss{});
    std::cout << "Newbee: "<< res << std::endl;
    static_assert(res - 0.6666 < 1e-4 and res - 0.6666 >= 0);

    std::cout << Integrate<square, Order33>() << std::endl;
    static_assert(Integrate<square, Order33>() - 0.6666 < 1e-4 and Integrate<square, Order33>() - 0.6666 >= 0);

    std::cout << Integrate<square, Order55>() << std::endl;
    static_assert(Integrate<square, Order55>() - 0.6666 < 1e-4 and Integrate<square, Order55>() - 0.6666 >= 0);

    return 0;
}

