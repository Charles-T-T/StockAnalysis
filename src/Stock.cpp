#include "../include/Stock.h"

Stock::Stock() : tsCode(""), tradeDate(0), open(0), high(0), low(0), close(0), preClose(0), change(0), pctChg(0), vol(0), amount(0) {}

Stock::Stock(std::string dataLine){
    std::stringstream ss(dataLine);
    std::getline(ss, tsCode, ',');
    std::string temp;
    std::getline(ss, temp, ',');
    tradeDate = std::stoi(temp);
    std::getline(ss, temp, ',');
    open = std::stod(temp);
    std::getline(ss, temp, ',');
    high = std::stod(temp);
    std::getline(ss, temp, ',');
    low = std::stod(temp);
    std::getline(ss, temp, ',');
    close = std::stod(temp);
    std::getline(ss, temp, ',');
    preClose = std::stod(temp);
    std::getline(ss, temp, ',');
    change = std::stod(temp);
    std::getline(ss, temp, ',');
    pctChg = stod(temp);
    std::getline(ss, temp, ',');
    vol = std::stod(temp);
    std::getline(ss, temp, ',');
    amount = std::stod(temp);
}

void Stock::Display() const{
    std::cout << "Stock[" << tsCode << "]" << std::endl;
    std::cout << "Trade date: " << tradeDate << std::endl;
    std::cout << "Open: " << open << std::endl;
    std::cout << "Close: " << close << std::endl;
    std::cout << "preClose: " << preClose << std::endl;
    std::cout << "High: " << high << std::endl;
    std::cout << "Low: " << low << std::endl;
    std::cout << "Change: " << change << std::endl;
    std::cout << "PctChg: " << pctChg << std::endl;
    std::cout << "Volumn: " << vol << std::endl;
    std::cout << "Amount: " << amount << std::endl;
}

bool Stock::operator==(const Stock& other) const{
    return tsCode == other.tsCode && tradeDate == other.tradeDate;
}
 


// 320682