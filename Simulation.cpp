#include "Simulation.hpp"
#include <iostream>
#include <vector>
#include <sstream>
#include <algorithm>
#include <ctime>
#include "OptionPrice.hpp"
#include "DeltaHedging.hpp"

Simulation::Simulation(std::vector<InterestData> interestRates,
                       std::vector<StockData> stockPrices,
                       std::vector<OptionData> optionPrices) {
    this->interestRates = interestRates;
    this->stockPrices = stockPrices;
    this->optionPrices = optionPrices;
}

std::tm Simulation::stringToTm(const std::string &s) {
        std::tm tm = {};
        std::istringstream ss(s);
        ss >> std::get_time(&tm, "%Y-%m-%d");
        return tm;
}

bool Simulation::isBetween(const std::string &date, const std::string &start, const std::string &end) {
    std::tm d = stringToTm(date);
    std::tm s = stringToTm(start);
    std::tm e = stringToTm(end);
    
    // Convert to time_t for comparison
    std::time_t dt = std::mktime(&d);
    std::time_t st = std::mktime(&s);
    std::time_t et = std::mktime(&e);

    return dt >= st && dt <= et;
}

std::vector<std::string> Simulation::findDatesInRange(const std::string &d1, const std::string &d2) {
    std::vector<std::string> result;
    for (const auto &entry : interestRates) {
        if (isBetween(entry.date, d1, d2)) {
            result.push_back(entry.date);
        }
    }
    return result;
};

std::vector<double> Simulation::findInterestRatesInRange(const std::string &d1, const std::string &d2) {
    std::vector<double> result;
    for (const auto &entry : interestRates) {
        if (isBetween(entry.date, d1, d2)) {
            result.push_back(entry.rate);
        }
    }
    return result;
}

std::vector<double> Simulation::findStockPricesInRange(const std::string &d1, const std::string &d2) {
    std::vector<double> result;
    for (const auto &entry : stockPrices) {
        if (isBetween(entry.date, d1, d2)) {
            result.push_back(entry.closePrice);
        }
    }
    return result;
}

std::vector<double> Simulation::findOptionPricesInRange(const std::string &d1, const std::string &d2, const std::string &expireDate, double K, char flag) {
    std::vector<double> result;
    for (const auto &entry : optionPrices) {
        if (isBetween(entry.date, d1, d2) && entry.expireDate == expireDate && entry.strikePrice == K && flag == entry.flag) {
            result.push_back(entry.price);
        }
    }
    return result;
}

int Simulation::generateOutout(std::string t0, std::string tN, std::string expireDate, double K, char flag) {
    std::vector<std::string> dates_op;
    std::vector<std::string> all_dates;
    std::vector<double> stockPrices_op;
    std::vector<double> interest_op;
    std::vector<double> timeToMaturity;
    std::vector<double> optionPrices_op;
    std::vector<double> implied_volality;
    std::vector<double> pnl_op;
    std::vector<double> delta_op;
    std::vector<double> hedgingError_op;
    std::vector<double> pnl_hedge_op;

    // Number of trading days
    dates_op = findDatesInRange(t0, tN);
    all_dates = findDatesInRange(t0, expireDate);
    interest_op = findInterestRatesInRange(t0, tN);
    stockPrices_op = findStockPricesInRange(t0, tN);
    optionPrices_op  = findOptionPricesInRange(t0, tN, expireDate, K, flag);
    double N = all_dates.size() / 255.0;
    // for (const auto &c : optionPrices) {
    //     std::cout << c.date << std::endl;
    //     std::cout << c.expireDate << std::endl;
    //     std::cout << c.strikePrice << std::endl;
    // }
    // This part is good. Ignore

    // Find the PNL;
    for (int i = 0; i < optionPrices_op.size(); i++) {
        pnl_op.push_back(optionPrices_op[i] - optionPrices_op[0]);
    }

    // Find the implied volatility
    for (int i = 0; i < optionPrices_op.size(); i++) {
        OptionPrice optionPrice(K, flag);
        double volt = optionPrice.findImpliedVolatity(stockPrices_op[i], interest_op[i], N - i*(1/255.0), optionPrices_op[i]);
        implied_volality.push_back(volt);
    }
    // std::cout << "Date size " << dates_op.size() << std::endl;
    // std::cout << "Stockprice size " << stockPrices_op.size() << std::endl;
    // std::cout << "Interest size " << interest_op.size() << std::endl;
    // std::cout << "Option price size " << optionPrices_op.size() << std::endl;
    // std::cout << "PNL size " << pnl_op.size() << std::endl;

    // Find the delta
    for (int i = 0; i < stockPrices_op.size(); i++) {
        OptionPrice optionPrice(K, flag);
        std::pair<double, double> res =  optionPrice.BSMPricerSingle(stockPrices_op[i], interest_op[i], N - i*(1/255.0), implied_volality[i]);
        delta_op.push_back(res.first);
    }

    // Find the hedging error
    DeltaHedging deltaHedging(stockPrices_op, optionPrices_op, interest_op, delta_op, 1/255.0);
    hedgingError_op = deltaHedging.calculateHedgingErrors();
    pnl_hedge_op = hedgingError_op;


    // Output data as result.csv
    std::ofstream outFile("result/result.csv");
    if (!outFile.is_open()) {
        std::cerr << "Error: Could not open output file!" << std::endl;
        return -1;
    }
    outFile << "date,S,V,implied volatility,delta,hedgingError,PNL,PNL (with hedge)\n";
    for (std::size_t i = 0; i < stockPrices_op.size(); ++i) {
        outFile << dates_op[i] << ","
                << stockPrices_op[i] << ","
                << optionPrices_op[i] << ","
                << implied_volality[i] << ","
                << delta_op[i] << ","
                << hedgingError_op[i] << ","
                << pnl_op[i] << ","
                << pnl_hedge_op[i]
                << "\n";
    }
    outFile.close();
    std::cout << "CSV file generated successfully!" << std::endl;
    return 0;
}

// If you have more methods or additional logic, add them here
