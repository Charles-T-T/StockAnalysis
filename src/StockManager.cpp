#include "./include/StockManager.h"

StockManager::StockManager(std::string dataPath, int maxMemory)
{
    stockDataPath = dataPath;
    sortMemory = maxMemory;
    numSortOnce = sortMemory / sizeof(Stock);
}

void StockManager::ReadData2Buf()
{
    stockBuffer.clear();
    // stockBuffer.resize(numSortOnce);

    // 逐行读取股票信息并写入缓存区
    std::string line;
    for (int i = 0; i < numSortOnce && std::getline(stockFile, line); i++)
    {
        std::stringstream ss(line);
        Stock stock(line);
        stockBuffer.push_back(stock);
    }
}

std::string StockManager::WriteBuf2Temp(int fileID)
{
    std::string fileName = "temp_" + std::to_string(fileID) + ".txt";
    std::ofstream outFile(fileName);

    // 将缓存区内容写入临时文件
    for (const Stock &stock : stockBuffer)
    {
        outFile << stock.tsCode << "," << stock.tradeDate << "," << stock.open << "," << stock.high << "," << stock.low << "," << stock.close << "," << stock.preClose << "," << stock.change << "," << stock.pctChg << "," << stock.vol << "," << stock.amount << std::endl;
    }
    outFile.close();
    tempFileNames.push_back(fileName);
    return fileName;
}

void StockManager::MergeFilesSort()
{
    
}

void StockManager::ExternalSort()
{
    stockFile.open(stockDataPath);
    if (!stockFile.is_open())
    {
        std::cerr << "Error: Unable to open stock data file: " << stockDataPath << std::endl;
        return;
    }

    // 分块排序
    int tempFID = 0;
    while (!stockFile.eof())
    {
        ReadData2Buf();
        std::sort(stockBuffer.begin(), stockBuffer.end(), CmpStock());
    }
}


/* TEST */
void StockManager::TestReadWrite(){
    ReadData2Buf();
    std::string tempFileName = WriteBuf2Temp(2024);
    std::cout << "Finish reading and writing!" << std::endl;
}

void StockManager::Openfile(){
    stockFile.open(stockDataPath);
    if (!stockFile.is_open()){
        std::cerr << "fail to open file!" << std::endl;
        return;
    }
    std::string line;
    std::getline(stockFile, line);
    std::cout << "Open file successfully!" << std::endl;
}