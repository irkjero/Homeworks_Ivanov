
#include <iostream>
#include <cstdint>

#include <tuple>


template <int current>
struct Order3{
    constexpr static std::tuple points_weights = {0.0, 8.0/9.0, 0.77459, 5.0/9.0, -0.77459, 5.0/9.0};
    constexpr static int size = 6;
    constexpr static int counter = 0;
    constexpr static double get_elem(){
        return std::get<counter>(points_weights);
    }
};


template<typename Tuple_type, typename Function, int current, int size>
struct intt{
    static double integral(const Tuple_type &tuple, Function func) {
//        std::cout << current;
        double p = std::get<current>(tuple);
        double w = std::get<current+1>(tuple);
        std::cout << w << p << " func :: " <<func(p)*w <<std::endl;
        return func(p)*w + intt<Tuple_type, Function, current+2, size>::integral(tuple, func);
    }
};

template<typename Tuple_type, typename Function, int size>
struct intt<Tuple_type, Function, size, size>{
    static double integral(const Tuple_type &tuple, Function func) {

        return 0;
    }
};


template<typename argument>
constexpr argument my_function(argument x) {
    return x * x;//- 7 * (1 + x * x / 2 + x * x * x * x / 24);
}

template<typename Function>
constexpr double Integrate(Function my_function) {
    constexpr int32_t leftNode = -5;
    constexpr int32_t rightNode = 5;

    return (rightNode - leftNode) / 6.0 * (my_function(leftNode) + 4 * my_function((leftNode + rightNode) / 2.0) +
                                           my_function(rightNode));


}


int main() {
    constexpr auto f = my_function<double>;
    //auto result = Integrate(f);
    std::cout << Integrate(f) <<std::endl;
    static_assert(Integrate(f)-83.333 < 1e-3 and Integrate(f)-83.333 >= 0, "good");
    constexpr const std::tuple<double,double,double,double,double,double> points_weights = {0.0, 8.0/9.0, 0.77459, 5.0/9.0, -0.77459, 5.0/9.0};
//    constexpr
    std::cout << intt<std::tuple<double,double,double,double,double,double>, std::function<double(double)>, 0, 6>::integral(points_weights, f);

    static_assert(intt<std::tuple<double,double,double,double,double,double>, std::function<double(double)>, 0, 6>::integral(points_weights, f) - 0.666 < 1e-3);
    return 0;
}

