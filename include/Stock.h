#include <iostream>
#include <vector>
#include <fstream>
#include <sstream>
#include <algorithm>
#include <queue>
#include <string>
#include <stack>
#include <utility>
#include <cstdio>
#include <chrono>

#pragma once

class StockManager;
class Stock
{
private:
    std::string tsCode; // 股票代码
    int tradeDate;      // 交易日期
    double open;        // 开盘价
    double high;        // 最高价
    double low;         // 最低价
    double close;       // 收盘价
    double preClose;    // 临近收盘价
    double change;      // 涨跌幅
    double pctChg;      // 涨跌百分比
    double vol;         // 交易量
    double amount;      // 交易金额

public:
    Stock();
    Stock(std::string dataLine);                 // 根据一行股票数据构造Stock对象
    void WriteToFile(std::ofstream &file) const; // 将一行股票数据写入文件
    friend class StockManager;
    friend struct CmpStock;
    friend struct CmpPair;
    void Display();
    bool operator==(const Stock &other) const;
};

struct CmpStock // 用于按要求对股票进行排序
{
    bool operator()(const Stock &a, const Stock &b)
    {
        // 先按股票代码字典序排序
        if (a.tsCode != b.tsCode)
            return a.tsCode < b.tsCode;
        // 股票代码相同则按日期排序
        else
            return a.tradeDate < b.tradeDate;
    }
};

struct CmpPair
{
    bool operator()(const std::pair<Stock, int> &p1, const std::pair<Stock, int> &p2)
    {
        if (p1.first.tsCode != p2.first.tsCode)
            return p1.first.tsCode < p2.first.tsCode;
        else
            return p1.first.tradeDate < p2.first.tradeDate;
    }
};

inline void Stock::WriteToFile(std::ofstream &file) const
{
    file << tsCode << "," << tradeDate << "," << open << "," << high << "," << low << "," << close << "," << preClose << "," << change << "," << pctChg << "," << vol << "," << amount << std::endl;
}