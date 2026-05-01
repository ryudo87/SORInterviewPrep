#include <iostream>
#include <vector>
using namespace std;

//A) if constexpr
template <typename T>
void printKind(const T& x) {
    if constexpr (std::is_integral_v<T>) {
        std::cout << "integral: " << x << "\n";
    } else {
        std::cout << "non-integral\n";
    }
}

//B) fold expressions
//Elegant variadic template reduction.
template <typename... Args>
auto sum(Args... args) {
    return (args + ...); // unary right fold
}

//C) template specialization
template <auto N>
struct ConstValue {
    static constexpr auto value = N;
};

template <typename T>
inline constexpr bool is_int_v = std::is_same_v<T, int>;

// What is SFINAE? SFINAE stands for "Substitution Failure Is Not An Error". 
// It is a C++ template metaprogramming technique that allows the compiler to 
// ignore certain template instantiations that fail during substitution, 
// rather than treating them as errors. 

//D) SFINAE example
template <typename T>
typename std::enable_if<std::is_integral_v<T>, void>::type
process(T x) {
    std::cout << "Processing integral: " << x << "\n";
}


int main() {
    std::cout << "ConstValue<>::value = " << ConstValue<33>::value << "\n";
    std::cout << "is_int_v<int> = " << is_int_v<int> << "\n";
    return 0;
}
