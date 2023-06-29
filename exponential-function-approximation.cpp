#include <cstdint>
#include <limits>
#include <iostream>

uint64_t F(uint64_t n) { if (n == 0) return 1;

    uint64_t result = 1;
    for (auto i = 1; i <= n; i++)
        result = result*i;

    return result;
}

long double P(long double x, uint64_t n) { if (n == 0) return 1.0;

    long double result = 1;
    for (auto i = 1; i <= n; i++)
        result = result*x;

    return result;
}

long double exp2(long double x) { 

    long double result = 1.0, term = 0.0;
    int digits = std::numeric_limits<long double>::digits10;
    long double precision = 1;
    
    for (auto i = 0; i < digits; i++)
        precision *= 0.1;

    uint64_t i = 1;

    do {
        term = P(x, i) / F(i);
        result += term;
        i++;
       } 
    while (term >= precision);

    return result;
}

int main() {

    long double x;
    std::cout << "Enter a x value: ";
    std::cin >> x;
    std::cout << "Approximation for exp(x) = " << x << " is " << exp2(x) << std::endl;

    return 0;
}
