#ifndef CSV_READER_HPP
#define CSV_READER_HPP
#include <vector>
#include <fstream>
#include <sstream>
#include <string>
#include <iostream>

struct InterestData {
    std::string date;
    double rate;
};

struct OptionData {
    std::string date;
    std::string expireDate;
    char flag;
    double strikePrice;
    double price;
};

struct StockData {
    std::string date;
    double closePrice;
};

class CSVReader {
    private:
        std::string interestFilePath;
        std::string optionFilePath;
        std::string stockFilePath;
    public:
        CSVReader() {
            this->interestFilePath = "./data/interest.csv";
            this->optionFilePath = "./data/op_GOOG.csv";
            this->stockFilePath = "./data/sec_GOOG.csv";
        }

        std::vector<InterestData> getInterestData() {
            std::ifstream file1(interestFilePath); // Replace with the path to your file
            if (!file1.is_open()) {
                std::cerr << "Failed to open the file!" << std::endl;
            }
            std::string line;
            std::vector<InterestData> interestRates;
            // Skip the header line
            std::getline(file1, line);
            double tempRate = 0.0;
            while (std::getline(file1, line)) {
                std::istringstream ss(line);
                InterestData entry;
                std::getline(ss, entry.date, ',');
                ss >> tempRate;
                entry.rate = tempRate / 100.0;
                interestRates.push_back(entry);
            }
            return interestRates;
        }

        std::vector<OptionData> getOptionData() {
            std::ifstream file(optionFilePath); // Replace with the path to your file
            if (!file.is_open()) {
                std::cerr << "Failed to open the file!" << std::endl;
            }
            std::vector<OptionData> optionDatas;
            std::string line;
            // Skip the header line
            std::getline(file, line);
            while (std::getline(file, line)) {
                double best_bid;
                double best_offer;
                std::istringstream ss(line);
                OptionData data;
                std::getline(ss, data.date, ',');
                std::getline(ss, data.expireDate, ',');
                ss >> data.flag;
                ss.ignore();
                ss >> data.strikePrice;
                ss.ignore();
                ss >> best_bid;
                ss.ignore();
                ss >> best_offer;
                data.price = (best_bid + best_offer) / 2;
                optionDatas.push_back(data);
            }
            return optionDatas;
        }

        std::vector<StockData> getStockData() {
            std::ifstream file(stockFilePath); // Replace with the path to your file
            if (!file.is_open()) {
                std::cerr << "Failed to open the file!" << std::endl;
            }
            std::vector<StockData> stockDatas;
            std::string line;
            // Skip the header line
            std::getline(file, line);
            while (std::getline(file, line)) {
                std::istringstream ss(line);
                StockData data;
                std::getline(ss, data.date, ',');
                ss >> data.closePrice;
                stockDatas.push_back(data);
            }
        return stockDatas;
        }
};
#endif // CSV_READER_HPP
