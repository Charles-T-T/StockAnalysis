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
    Stock(std::string dataLine); // ��һ�й�Ʊ���ݹ���Stock����
    friend class StockManager;
    friend struct CmpStock;
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