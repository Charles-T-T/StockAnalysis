#include <iostream>
#include <vector>
#include <fstream>
#include <sstream>
#include <algorithm>
#include <queue>

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
    Stock(std::string dataLine); // 从一行股票数据构造Stock对象
    friend class StockManager;
    friend struct CmpStock;
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