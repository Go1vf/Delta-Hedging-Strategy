#include "DeltaHedging.hpp"
#include <vector>
#include <cmath>
#include <iostream>

DeltaHedging::DeltaHedging(std::vector<double> stockPrices, std::vector<double> optionPrices, std::vector<double> interestRates, std::vector<double> deltas, double deltaT) {
    this->stockPrices = stockPrices;
    this->interestRates = interestRates;
    this->deltas = deltas;
    this->deltaT = deltaT;
    this->optionPrices = optionPrices;
    int N = stockPrices.size();
    std::vector <double> portfolioValues;
    double position;
    // calculate portfolio values
    for (int i = 0; i < N; i++) {
        if (i == 0) {
            position = optionPrices[0] - (deltas[0] * stockPrices[0]);
        } else {
            position = deltas[i-1] * stockPrices[i] + (portfolioValues[i-1] * exp(interestRates[i-1] * deltaT)) - (deltas[i]*stockPrices[i]);
        }
        portfolioValues.push_back(position);
    }
    this->portfolioValues = portfolioValues;
}

std::vector<double> DeltaHedging::calculateHedgingErrors() {
    std::vector<double> hedgingErrors;
    int N = stockPrices.size();
    hedgingErrors.push_back(0.0);
    for (int i = 1; i < N; i++) {
        double hedgingError = deltas[i-1] * stockPrices[i] + (portfolioValues[i-1] * exp(interestRates[i-1] * deltaT)) - optionPrices[i];
        hedgingErrors.push_back(hedgingError);
    }
    return hedgingErrors;
}

std::vector<double> DeltaHedging::getPortfolioValues() {
    return portfolioValues;
}

