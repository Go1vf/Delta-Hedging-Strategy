#ifndef STOCK_PRICE_HPP
#define STOCK_PRICE_HPP
#include <vector>
#include <random>

class StockPrice {
    public:
        StockPrice(); // default constructor
        StockPrice(double initialPrice, double timeToMaturity, double volatility, double riskFreeRate, double u, int N); // paramterized constructor
        std::vector<double> calculateStockPrice(std::vector<double> randomProbs);
        void setInitialPrice(double initialPrice);
        void setTimeToMaturity(double timeToMaturity);
        void setVolatility(double volatility);
        void setRiskFreeRate(double riskFreeRate);
        void setMean(double mean);
    private:
        double initialPrice;
        double timeToMaturity;
        double volatility;
        double riskFreeRate;
        double mean;
        int N;
};
#endif // STOCK_PRICE_HPP