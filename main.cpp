#include "StockPrice.hpp"
#include "OptionPrice.hpp"
#include "DeltaHedging.hpp"
#include <iostream>
#include <vector>
#include <fstream>
#include <sstream>
#include <boost/foreach.hpp>
#include <random>
#include "CSVReader.cpp"
#include "Simulation.hpp"
#include "UnitTest/Test.cpp"

int main() {
    double S0 = 100.0; // Initial stock price
    double T = 0.4; // Time to maturity
    double mean = 0.05; // Risk-free rate
    double sigma = 0.24; // Volatility
    double r = 0.025; // Risk-free rate
    int N = 101; // Number of time steps

    double K = 105.0; // Strike price
    char flag = 'C';

    // Create a random number generator
    std::mt19937 generator(42);
    std::normal_distribution<double> distribution(0.0, 1.0);

    // Create 1000 different sample paths
    StockPrice stockPrice(S0, T, sigma, r, mean, N);
    std::vector<std::vector<double>> stockPriceCollections;

    for (int i = 0; i < 1000; i ++) {
        std::vector<double> randomProbs;
        for (int i = 0; i < N; i++) {
            randomProbs.push_back(distribution(generator));
        }
        std::vector<double> stockPrices = stockPrice.calculateStockPrice(randomProbs);
        stockPriceCollections.push_back(stockPrices);
    }

    // Take the first 100 sample paths and output it as a csv file
    // std::ofstream outputFile1("stockPricePaths.csv");
    // if (!outputFile1.is_open()) {
    //     std::cerr << "Error opening file" << std::endl;
    //     return 1;
    // }
    // for (int i = 0; i < 100; i++) {
    //     for (int j = 0; j < N; j++) {
    //         outputFile1 << stockPriceCollections[i][j];
    //         if (j < stockPriceCollections[i].size() - 1) {
    //             outputFile1 << ",";  // separate values with commas
    //         }
    //     }
    //     outputFile1 << std::endl; // separate rows with newline characters
    // }

    // Apply BSM Formula to price options
    std::vector<std::vector<double>> optionPriceCollections;
    std::vector<std::vector<double>> deltaCollections;
    std::vector<double> rates(N, r);
    std::vector<double> sigmas(N, sigma);
    BOOST_FOREACH(std::vector<double> stockPrices, stockPriceCollections) {
        OptionPrice optionPrice(K,flag);
        std::pair<std::vector<double>, std::vector<double>> optionPriceAndDelta = optionPrice.BSMPricerGroup(stockPrices, rates, T, sigmas, T / (N-1));
        optionPriceCollections.push_back(optionPriceAndDelta.second);
        deltaCollections.push_back(optionPriceAndDelta.first);
    }

    // // Take the first 100 option prices and output it as a csv file
    // std::ofstream outputFile2("optionPricePaths.csv");
    // if (!outputFile2.is_open()) {
    //     std::cerr << "Error opening file" << std::endl;
    //     return 1;
    // }
    // for (int i = 0; i < 100; i++) {
    //     for (int j = 0; j < N; j++) {
    //         outputFile2 << optionPriceCollections[i][j];
    //         if (j < optionPriceCollections[i].size() - 1) {
    //             outputFile2 << ",";  // separate values with commas
    //         }
    //     }
    //     outputFile2 << std::endl; // separate rows with newline characters
    // }
    
    // deltaCollections
    // optionPriceCollections
    // stockPriceCollections
    std::vector<std::vector<double>> hedgingErrorCollections;
    for (int i = 0; i < 1000; i++) {
        DeltaHedging deltaHedging(stockPriceCollections[i], optionPriceCollections[i], rates, deltaCollections[i], T/N);
        std::vector<double> hedgingErrors = deltaHedging.calculateHedgingErrors();
        hedgingErrorCollections.push_back(hedgingErrors);
    }

    // Output hedgingErrorCollections as a csv file
    // std::ofstream outputFile3("hedgingErrorPaths.csv");
    // if (!outputFile3.is_open()) {
    //     std::cerr << "Error opening file" << std::endl;
    //     return 1;
    // }
    // for (int i = 0; i < 1000; i++) {
    //     for (int j = 0; j < N - 1; j++) {
    //         outputFile3 << std::fixed << std::setprecision(5) << hedgingErrorCollections[i][j];
    //         outputFile3 << ",";  // separate values with commas
    //     }
    //     outputFile3 << std::endl; // separate rows with newline characters
    // }
    
    // Read in interest.csv， op_GOOG.csv and stock_GOOG.csv
    CSVReader csvReader;
    std::vector<InterestData> interestRates = csvReader.getInterestData();
    std::vector<StockData> stockPrices = csvReader.getStockData();
    std::vector<OptionData> optionPrices = csvReader.getOptionData();

    //print optionPrices
    // for (const auto &c: optionPrices) {
    //     std::cout << c.date << std::endl;
    //     std::cout << c.expireDate << std::endl;
    //     std::cout << c.price << std::endl;
    //     std::cout << c.flag << std::endl;
    //     break;
    // }

    // Issue
    Simulation simulation(interestRates, stockPrices, optionPrices);
    simulation.generateOutout("2011-07-05", "2011-07-29", "2011-09-17", 500.0, 'C');

    // Test
    testCalculateImpliedVolatility();
    testDelta();
    return 0;
}