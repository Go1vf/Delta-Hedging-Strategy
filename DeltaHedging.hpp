#include <vector>

#ifndef DeltaHedging_HPP
#define DeltaHedging_HPP
class DeltaHedging {
    public:
        DeltaHedging(std::vector<double> stockPrices, std::vector<double> optionPrices, std::vector<double> interestRates, std::vector<double> deltas, double deltaT);
        std::vector<double> calculateHedgingErrors();
        std::vector<double> getPortfolioValues();
        
    private:
        std::vector<double> portfolioValues;
        std::vector<double> optionPrices;
        std::vector<double> stockPrices;
        std::vector<double> interestRates;
        std::vector<double> deltas;
        double deltaT;
};
#endif // DeltaHedging_HPP
