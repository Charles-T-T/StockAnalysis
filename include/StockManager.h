#pragma once

#include "Stock.h"

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
};
