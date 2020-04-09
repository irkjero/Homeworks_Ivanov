
#include <iostream>
#include <cstdint>

#include <tuple>


template <int current>
struct Order3{
    constexpr static std::tuple points_weights = {0.0, 8.0/9.0, 0.77459, 5.0/9.0, -0.77459, 5.0/9.0};
    constexpr static double get_elem(){
        return std::get<current>(points_weights);
    }
};


template<typename Function, int current, int size>
struct intt{
    static constexpr double integral(Function func) {
        constexpr double p = Order3<current>::get_elem();//std::get<current>(tuple);
        constexpr double w = Order3<current+1>::get_elem();//std::get<current+1>(tuple);

        return func(p)*w + intt<Function, current+2, size>::integral(func);
    }
};

template< typename Function, int size>
struct intt<Function, size, size>{
    static constexpr double integral(Function func) {
        return 0;
    }
};


template<typename argument>
constexpr argument my_function(argument x) {
    return x * x * x;//- 7 * (1 + x * x / 2 + x * x * x * x / 24);
}

template<typename Function>
constexpr double Integrate(Function my_function) {
    constexpr int32_t leftNode = -5;
    constexpr int32_t rightNode = 5;

    return (rightNode - leftNode) / 6.0 * (my_function(leftNode) + 4 * my_function((leftNode + rightNode) / 2.0) +
                                           my_function(rightNode));

}


struct Order33{
    constexpr static std::tuple points = {0.0, 0.77459, -0.77459};
    constexpr static std::tuple weights =  {8.0/9.0,  5.0/9.0, 5.0/9.0};
    constexpr static int order = 3;
};

struct NewFunc{
    constexpr static double my_function(double x) {
        return x * x;//- 7 * (1 + x * x / 2 + x * x * x * x / 24);
    }
};

template<typename Function , typename ord, int current, int order>
struct intrrr{
    constexpr static double Integrates() {
        return Function::my_function((std::get<current>(ord::points)))*std::get<current>(ord::weights) + intrrr<Function, ord, current+1, order>::Integrates();
    }
};

template<typename Function , typename ord, int order>
struct intrrr<Function, ord, order, order>{
    constexpr static double Integrates() {
        return 0;
    }
};

template<typename Function , typename ord>
constexpr double Integrates() {
    return intrrr<Function, ord, 0, ord::order>::Integrates();
}



int main() {
    constexpr auto f = my_function<double>;
    //auto result = Integrate(f);
    std::cout << Integrate(f) <<std::endl;
//    static_assert(Integrate(f)-83.333 < 1e-3 and Integrate(f)-83.333 >= 0, "good");
    constexpr const std::tuple<double,double,double,double,double,double> points_weights = {0.0, 8.0/9.0, 0.77459, 5.0/9.0, -0.77459, 5.0/9.0};
//    constexpr
    std::cout << intt<std::function<double(double)>, 0, 6>::integral(f) << std::endl;
//    constexpr bool oooo = intt<std::function<double(double)>, 0, 6>::integral(f) > 0;
//    static_assert(oooo);
    std::cout << Integrates<NewFunc, Order33>() << std::endl;
    static_assert(Integrates<NewFunc, Order33>() - 0.66666 < 1e-3);
    static_assert(std::get<0>(points_weights) == 0);
    return 0;
}

