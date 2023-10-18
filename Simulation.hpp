#ifndef SIMULATION_HPP
#define SIMULATION_HPP

#include "CSVReader.cpp"
#include <vector>
#include <random>
#include <utility>
class Simulation {
    private:
        std::vector<InterestData> interestRates;
        std::vector<StockData> stockPrices;
        std::vector<OptionData> optionPrices;
    public:
        Simulation(std::vector<InterestData> interestRates, std::vector<StockData> stockPrices, std::vector<OptionData> optionPrices);
        bool isBetween(const std::string &date, const std::string &start, const std::string &end);
        std::tm stringToTm(const std::string &s);
        std::vector<std::string> findDatesInRange(const std::string &d1, const std::string &d2);
        std::vector<double> findInterestRatesInRange(const std::string &d1, const std::string &d2);
        std::vector<double> findStockPricesInRange(const std::string &d1, const std::string &d2);
        std::vector<double> findOptionPricesInRange(const std::string &d1, const std::string &d2, const std::string &expireDate, double K, char flag);
        int generateOutout(std::string t0, std::string tN, std::string expireDate, double K, char flag);

};
#endif // SIMULATION_HPP