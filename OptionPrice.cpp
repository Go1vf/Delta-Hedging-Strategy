#include "OptionPrice.hpp"
#include <cmath>
#include <boost/math/distributions/normal.hpp>
#include <boost/foreach.hpp>
#include <utility>
#include <iostream>
#include <vector>

OptionPrice::OptionPrice(double K, char flag)
{
    this->flag = flag; //// 'C' for call option and 'P' for put option
    this->K = K;
}

// stockPrices: vector
// r: double
// T: double
// sigma: double
// flag: char
// K: double
std::pair<std::vector<double>, std::vector<double>> OptionPrice::BSMPricerGroup(std::vector<double> stockPrices, std::vector<double> r, double T, std::vector<double> sigma, double deltaT) {
    std::vector<double> prices;
    std::vector<double> deltas;
    // std::cout << deltaT << std::endl;
    double t = T;
    int i = 0;

    for (int i = 0; i < stockPrices.size(); i++) {
        double S = stockPrices[i];
        double price = -1.0;
        double delta = -1.0;
        boost::math::normal_distribution<> standardNormal;
        auto N = [&](double x) { return boost::math::cdf(standardNormal, x); };
        t = T - i * deltaT;
        // std::cout << "t: " << t << std::endl;

        double d1 = (log(S / K) + (r[i] + 0.5 * pow(sigma[i], 2)) * t) / (sigma[i] * sqrt(t));
        double d2 = d1 - sigma[i] * sqrt(t);

        if (flag == 'C' ) {
            delta = N(d1);
            price = S * delta - K * exp(-r[i] * (t)) * N(d2);
        } else if (flag == 'P') {
            delta = N(d1) - 1;
            price = K * exp(-r[i] * (t)) * N(-d2) - S * N(-d1);
        } else {
            std::cout << "Invalid flag" << std::endl;
        }

        prices.push_back(price);
        deltas.push_back(delta);

    }
    return {deltas, prices};
}

std::pair<double, double> OptionPrice::BSMPricerSingle(double S, double r, double T, double sigma) {
    double price = -1.0;
    double delta = -1.0;
    boost::math::normal_distribution<> standardNormal;
    auto N = [&](double x) { return boost::math::cdf(standardNormal, x); };

    double d1 = (log(S / K) + (r + 0.5 * pow(sigma, 2)) * T) / (sigma * sqrt(T));
    double d2 = d1 - sigma * sqrt(T);

    if (flag == 'C' ) {
        delta = N(d1);
        price = S * delta - K * exp(-r * T) * N(d2);
    } else if (flag == 'P') {
        delta = N(d1) - 1;
        price = K * exp(-r * T) * N(-d2) - S * N(-d1);
    } else {
        std::cout << "Invalid flag" << std::endl;
    }

    return {delta, price};
}

double OptionPrice::findImpliedVolatity(double S, double r, double T, double targetOptionPrice) {
    double lower = 0.0;
    double upper = 2.0;
    double tol = 0.01;
    double mid;
    double price = -1.0;
    const int maxIterations = 1000;  // Set a maximum limit to prevent infinite loop
    int iteration = 0;
    double prevMid = -1;  // to store the previous mid value
    
    while (abs(price - targetOptionPrice) > tol ) {
        if (iteration >= maxIterations) {
            std::cout << "Maximum iterations reached!" << std::endl;
            break;
        }
        
        mid = (lower + upper) / 2.0;

        if (abs(mid - prevMid) < 1e-10) {  // If mid isn't changing, exit the loop
            std::cout << "Algorithm converging!" << std::endl;
            break;
        }

        price = BSMPricerSingle(S, r, T, mid).second;

        if (price > targetOptionPrice) {
            upper = mid;
        } else {
            lower = mid;
        }
        
        prevMid = mid;  // Update the previous mid value
        iteration++;    // Increment the iteration count
    }
    return mid;
}

