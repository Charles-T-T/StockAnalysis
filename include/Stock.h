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
    std::string tsCode; // ��Ʊ����
    int tradeDate;      // ��������
    double open;        // ���̼�
    double high;        // ��߼�
    double low;         // ��ͼ�
    double close;       // ���̼�
    double preClose;    // �ٽ����̼�
    double change;      // �ǵ���
    double pctChg;      // �ǵ��ٷֱ�
    double vol;         // ������
    double amount;      // ���׽��

public:
    Stock();
    Stock(std::string dataLine);                 // ����һ�й�Ʊ���ݹ���Stock����
    void WriteToFile(std::ofstream &file) const; // ��һ�й�Ʊ����д���ļ�
    friend class StockManager;
    friend struct CmpStock;
    friend struct CmpPair;
    void Display();
    bool operator==(const Stock &other) const;
};

struct CmpStock // ���ڰ�Ҫ��Թ�Ʊ��������
{
    bool operator()(const Stock &a, const Stock &b)
    {
        // �Ȱ���Ʊ�����ֵ�������
        if (a.tsCode != b.tsCode)
            return a.tsCode < b.tsCode;
        // ��Ʊ������ͬ����������
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