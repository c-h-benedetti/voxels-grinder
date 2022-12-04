#include <iostream>
#include "filter.hpp"

#define HANDLED(r) FilterAcceptedTypes::isHandled<decltype(r)>()

int main() {

    float f1 = 1.23;
    int i1 = 42;
    char c1 = 'a';

    if (HANDLED(f1)) {
        std::cout << f1 << std::endl;
    }

    if (HANDLED(i1)) {
        std::cout << i1 << std::endl;
    }

    if (HANDLED(c1)) {
        std::cout << c1 << std::endl;
    }

    return 0;
}