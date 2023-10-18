#include "StockPrice.hpp"
#include <vector>
#include <cmath>
#include <iostream>

StockPrice::StockPrice() {
    initialPrice = 100;
    timeToMaturity = 0.4;
    volatility = 0.24;
    riskFreeRate = 0.025;
    mean = 0.05;
    N = 100;
}

StockPrice::StockPrice(double initialPrice, double timeToMaturity, double volatility, double riskFreeRate, double mean, int N) {
    this->initialPrice = initialPrice;
    this->timeToMaturity = timeToMaturity;
    this->volatility = volatility;
    this->riskFreeRate = riskFreeRate;
    this->mean = mean;
    this->N = N;
}

std::vector<double> StockPrice::calculateStockPrice(std::vector<double> randomProbs) {
    std::vector<double> stock_prices;
    stock_prices.push_back(initialPrice);
    double delta_t = timeToMaturity / (N - 1);
    // std::cout << delta_t << std::endl;
    double price = initialPrice;
    for (int i = 1; i < N; i++) {
        price = price + mean*price*delta_t + volatility*price*sqrt(delta_t)*randomProbs[i];
        stock_prices.push_back(price);
    }
    return stock_prices;
}

void StockPrice::setInitialPrice(double initialPrice) {
    this->initialPrice = initialPrice;
};
void StockPrice::setTimeToMaturity(double timeToMaturity) {
    this->timeToMaturity = timeToMaturity;
};
void StockPrice::setVolatility(double volatility) {
    this->volatility = volatility;

};
void StockPrice::setRiskFreeRate(double riskFreeRate) {
    this->riskFreeRate = riskFreeRate;
};
void StockPrice::setMean(double mean){
    this->mean = mean;
};