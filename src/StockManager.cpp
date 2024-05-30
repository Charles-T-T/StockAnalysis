#include "../include/StockManager.h"
#include "../include/StockPQ.h"
#include "../include/Timer.h"
#include <filesystem>

namespace fs = std::filesystem;

constexpr int DEBUG_MOOD = 1;

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
        stock.WriteToFile(outFile);
    }
    outFile.close();
    tempFileNames.push_back(fileName);
    stockBuffer.clear();
    return fileName;
}

void StockManager::MergeFilesSort(int fileCount)
{
    std::vector<std::fstream> files(fileCount);
    // 用一个优先队列进行股票排序
    MyPQ<std::pair<Stock, int>, CmpPair> pq;
    // 打开各个待归并的临时文件，将每个文件的第一行（其中最小的）写入优先队列pq
    for (int i = 0; i < fileCount; i++)
    {
        files[i].open("temp_" + std::to_string(i) + ".txt");
        std::string dataLine;
        std::getline(files[i], dataLine);
        pq.push(std::make_pair(Stock(dataLine), i));
    }
    // 定义输出文件
    std::string outPath = "../res/output.txt";
    std::ofstream outFile(outPath);
    if (!outFile.is_open())
    {
        std::cerr << "output file open fail!" << std::endl;
    }

    int curMinFID; // 当前最小的股票信息（pq队头的）所属的文件编号
    std::string curDataLine;
    while (!pq.empty())
    {
        // TODO debug line
        Stock topStock = pq.top().first;

        pq.top().first.WriteToFile(outFile);
        curMinFID = pq.top().second;
        pq.pop();
        topStock = pq.top().first;

        if (std::getline(files[curMinFID], curDataLine))
        {
            // 从该文件中读取下一行股票数据
            pq.push(std::make_pair(Stock(curDataLine), curMinFID));
        }
        else
        {
            // 若该文件已读完，将其关闭并删除
            files[curMinFID].close();
            
            if (DEBUG_MOOD)
                std::cout << "merge temp_" << curMinFID << std::endl;
            
            if (!fs::remove("temp_" + std::to_string(curMinFID) + ".txt"))
            {
                std::cerr << "Fail to delete temp_" << curMinFID << ".txt!" << std::endl;
            }
        }
    }

    outFile.close();
    
    if (DEBUG_MOOD)
        std::cout << "Finish merging!" << std::endl;
}

void StockManager::ExternalSort()
{
    if (DEBUG_MOOD)
        std::cout << "Start externalSort" << std::endl;
    
    stockFile.open(stockDataPath);
    if (!stockFile.is_open())
    {
        std::cerr << "Error: Unable to open stock data file: " << stockDataPath << std::endl;
        return;
    }

    // 分块排序
    if (DEBUG_MOOD)
        std::cout << "Start divided sorting" << std::endl;

    Timer timer;
    
    int tempFID = 0;
    std::string dataLine;
    std::getline(stockFile, dataLine); // 读取表头
    while (std::getline(stockFile, dataLine))
    {
        // 读取数据到缓冲区
        if (DEBUG_MOOD)
            std::cout << "tempFID: " << tempFID << std::endl;

        stockBuffer.clear();
        stockBuffer.emplace_back(dataLine);
        for (int i = 1; i < numSortOnce; ++i)
        {
            if (std::getline(stockFile, dataLine))
                stockBuffer.emplace_back(dataLine);
            else
                break;
        }
        // 在缓冲区排序
        std::sort(stockBuffer.begin(), stockBuffer.end(), CmpStock());
        // 将缓冲区排好序的数据写入临时文件
        WriteBuf2Temp(tempFID++);
    }
    if (DEBUG_MOOD)
        std::cout << "Time for dividing sort: " << timer.elapsed() << "ms" << std::endl;
    timer.reset();

    // 归并排序
    std::cout << "Start merging" << std::endl;
    MergeFilesSort(tempFID);

    if (DEBUG_MOOD)
        std::cout << "Time for merging sort: " << timer.elapsed() << "ms" << std::endl;
}

void StockManager::GenerateIndexFile(std::string sortedFileName, std::string indexFileName) {
    std::ifstream stockFile(sortedFileName, std::ios::in | std::ios::binary);
    std::ofstream indexFile(indexFileName);

    if (!stockFile.is_open()) {
        std::cerr << "Failed to open output!" << std::endl;
        return;
    }

    if (!indexFile.is_open()){
        std::cerr << "Failed to open index!" << std::endl;
    }

    // TODO debug line
    if (1){
        std::cout << "Start generating index..." << std::endl;
    }

    std::vector<IndexItem> indexItems;

    std::string prevLine, curLine;
    std::streampos offset = 0;
    std::streampos prevOffset, curOffset;
    
    // 逐行扫描output，在需要的行起始位置为其建立索引
    // 先记录第一行的起始位置，然后读取第一行内容作为prevLine
    prevOffset = stockFile.tellg();
    std::getline(stockFile, prevLine);
    Stock prevStock(prevLine);
    int prevYM = prevStock.tradeDate / 100; // YM --> "Year & Month"

    while (true){   
        curOffset = stockFile.tellg();
        if (!std::getline(stockFile, curLine)){
            break;
        }
        Stock curStock(curLine);
        int curYM = curStock.tradeDate / 100;
        if (curStock.tsCode != prevStock.tsCode || curYM != prevYM) {
            // 记录上一条股票数据的偏移量
            IndexItem indexItem;
            indexItem.tsCode = prevStock.tsCode;
            indexItem.tradeYM = prevYM;
            indexItem.offset = prevOffset;
            indexItems.push_back(indexItem);

            prevStock = curStock;
            prevYM = curYM;
            prevOffset = curOffset;
        }
    }

    
    // 写入索引文件
    for (const auto& item : indexItems) {
        indexFile << item.tsCode << " " << item.tradeYM << " " << item.offset << std::endl;
    }

    stockFile.close();
    indexFile.close();
}

std::unordered_map<std::string, std::unordered_map<int, std::streampos>>StockManager::LoadIndexFile(std::string indexFileName) {
    std::unordered_map<std::string, std::unordered_map<int, std::streampos>> indexMap;
    std::ifstream indexFile(indexFileName);
    if (!indexFile.is_open()) {
        std::cerr << "Failed to open index file!" << std::endl;
        return indexMap;
    }

    std::string tsCode;
    int tradeYM;
    std::streamoff offset;

    
    while (indexFile >> tsCode >> tradeYM >> offset) {
        indexMap[tsCode][tradeYM] = std::streampos(offset);
        // std::cout << "cur 2nd size: " << indexMap[tsCode].size() << std::endl;
        // std::cout << "cur 1st size: " << indexMap.size() << "\n" << std::endl;
        
    }


    indexFile.close();

    // TODO debug line
    std::cout << "index map size: " << indexMap.size() << std::endl;

    return indexMap;
}

std::vector<Stock>StockManager::QueryStockData(
    std::string outputFileName, 
    const std::unordered_map<std::string, std::unordered_map<int, std::streampos>>& indexMap, 
    std::string tsCode, int yearMonth
) {
    std::vector<Stock> result;
    auto itStock = indexMap.find(tsCode);
    if (itStock == indexMap.end()) {
        std::cerr << "Stock code not found in index!" << std::endl;
        return result;
    }

    auto itMonth = itStock->second.find(yearMonth);
    if (itMonth == itStock->second.end()) {
        std::cerr << "Year-Month not found for the specified stock code in index!" << std::endl;
        return result;
    }

    std::streampos offset = itMonth->second;
    std::ifstream inputFile(outputFileName, std::ios::in | std::ios::binary);
    if (!inputFile.is_open()) {
        std::cerr << "Failed to open data file!" << std::endl;
        return result;
    }

    inputFile.seekg(offset);

    std::string line;
    while (std::getline(inputFile, line)) {
        Stock stock(line);
        if (stock.tsCode != tsCode || (stock.tradeDate / 100) != yearMonth) {
            break;
        }
        result.push_back(stock);
    }

    inputFile.close();
    return result;
}


/* TEST */
void StockManager::TestReadWrite()
{
    stockFile.open(stockDataPath);
    if (!stockFile.is_open())
    {
        std::cerr << "Error: Unable to open stock data file: " << stockDataPath << std::endl;
        return;
    }

    // 读取第一行（表头）
    std::string head;
    std::getline(stockFile, head);
    int tempFID = 0;
    while (!stockFile.eof())
    {
        ReadData2Buf();
        std::string filename = WriteBuf2Temp(tempFID);
        tempFID++;

        if (tempFID > 50)
            break;
    }
    
    if (DEBUG_MOOD)
        std::cout << "temp_" << tempFID << std::endl;

    std::cout << "finish reading and writing test!" << std::endl;

    stockFile.close();
}

void StockManager::Openfile()
{
    stockFile.open(stockDataPath);
    if (!stockFile.is_open())
    {
        std::cerr << "fail to open file!" << std::endl;
        return;
    }
    std::string line;
    std::getline(stockFile, line);
    std::cout << "Open file successfully!" << std::endl;
}

void StockManager::ShowBufferInfo()
{
    std::cout << "Buffer size: " << sortMemory << std::endl;

    std::cout << "Num once: " << numSortOnce << std::endl;
}