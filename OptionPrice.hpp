#include <utility>
#include <vector>

#ifndef OPTION_PRICE_HPP
#define OPTION_PRICE_HPP
class OptionPrice{
public:
    OptionPrice(double K, char flag); // Constructor to initialize flag
    std::pair<std::vector<double>, std::vector<double>> BSMPricerGroup(std::vector<double> stockPrices, std::vector<double> r, double T, std::vector<double> sigma, double deltaT);  // For task 1
    std::pair<double, double> BSMPricerSingle(double S, double r, double T, double sigma); // For task 2
    double findImpliedVolatity(double S, double r, double T, double targetOptionPrice);
    
private:
    char flag; // Private member to store flag
    double K;
};
#endif