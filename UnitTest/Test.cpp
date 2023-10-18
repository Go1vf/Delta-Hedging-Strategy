#include "../OptionPrice.hpp" 
#include <iostream>


void testCalculateImpliedVolatility() {
    OptionPrice option(500.0, 'C');
    double impliedVolatility = option.findImpliedVolatity(527.28, 0.17, 32.0/255.0, 41);
    if (abs(impliedVolatility - 0.21) < 0.01) {
        std::cout << "Test passed" << std::endl;
    } else {
        std::cout << "Test failed" << std::endl;
    }
}

void testDelta() {
    OptionPrice option(500.0, 'C');
    double delta = option.BSMPricerSingle(527.28, 0.17, 32.0/255.0, 0.2).first;
    if (delta - 0.861 < 0.01) {
        std::cout << "Test passed" << std::endl;
    } else {
        std::cout << "Test failed" << std::endl;
    }
}