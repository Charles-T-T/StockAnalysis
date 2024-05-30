#pragma once

#include "Stock.h"
#include <unordered_map>
#include <random>

class StockManager
{
private:
    int sortMemory;  // 外排序可用的最大内存
    int numSortOnce; // 每次处理的股票数
    std::vector<Stock> stockBuffer;
    std::string stockDataPath;
    std::ifstream stockFile;
    std::vector<std::string> tempFileNames;

public:
    StockManager(std::string dataPath, int maxMemory);
    // ~StockManager();

    void ReadData2Buf();                   // 读取一行股票信息到缓存
    std::string WriteBuf2Temp(int fileID); // 将缓存信息写入临时文件，返回文件名
    void MergeFilesSort(int fileCount);    // 归并排序各个临时文件
    void ExternalSort();                   // 外排总函数

    void TestReadWrite();
    void Openfile();
    void ShowBufferInfo();

    // 创建索引项的结构
    struct IndexItem {
        std::string tsCode;       // 股票代码
        int tradeYM;       // 交易年月
        std::streampos offset;    // 偏移量
    };

    // 生成索引文件
    void GenerateIndexFile(std::string utputFileName,std::string indexFileName);

    // 读取索引文件并返回一个unordered_map
    std::unordered_map<std::string, std::unordered_map<int, std::streampos>> LoadIndexFile(std::string indexFileName);

    // 利用索引查询指定股票某月的数据
    std::vector<Stock> QueryStockData(
        std::string outputFileName, 
        const std::unordered_map<std::string, std::unordered_map<int, std::streampos>>& indexMap, 
        std::string tsCode, int yearMonth
    );

};
