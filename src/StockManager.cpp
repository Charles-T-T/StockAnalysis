#include "StockManager.h"
#include <filesystem>

namespace fs = std::filesystem;

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
        stock.WriteToFile(outFile);
    }
    outFile.close();
    tempFileNames.push_back(fileName);
    return fileName;
}

void StockManager::MergeFilesSort(int fileCount)
{
    std::vector<std::fstream> files(fileCount);
    // 用一个优先队列进行股票排序
    std::priority_queue<std::pair<Stock, int>, std::vector<std::pair<Stock, int>>, CmpPair> pq; 
    // 打开各个待归并的临时文件，将每个文件的第一行（其中最小的）写入优先队列pq
    for (int i = 0; i < fileCount; i++)
    {
        files[i].open("temp_" + std::to_string(i) + ".txt");
        std::string dataLine;
        std::getline(files[i], dataLine);
        pq.push(std::make_pair(Stock(dataLine), i));
    }
    // 定义输出文件
    std::string outPath = "output.txt";
    std::ofstream outFile(outPath);
    if (!outFile.is_open()){
        std::cerr << "output file open fail!" << std::endl;
    }

    int curMinFID; // 当前最小的股票信息（pq队头的）所属的文件编号
    std::string curDataLine;
    while (!pq.empty())
    {
        pq.top().first.WriteToFile(outFile);
        curMinFID = pq.top().second;
        pq.pop();
        if (std::getline(files[curMinFID], curDataLine)){
            // 从该文件中读取下一行股票数据
            pq.push(std::make_pair(Stock(curDataLine), curMinFID));
        } else {
            // 若该文件已读完，将其关闭并删除
            // TODO 是否能直接删除？
            files[curMinFID].close();
            if (!fs::remove("temp_" + std::to_string(curMinFID) + ".txt")){
                std::cerr << "Fail to delete temp_" << curMinFID << ".txt!" << std::endl;
            }
        }
    }

    outFile.close();
    // TODO debug line
    std::cout << "Finish merging!" << std::endl;
}

void StockManager::ExternalSort()
{
    // TODO debug line
    std::cout << "Start externalSort" << std::endl;
    stockFile.open(stockDataPath);
    if (!stockFile.is_open())
    {
        std::cerr << "Error: Unable to open stock data file: " << stockDataPath << std::endl;
        return;
    }

    // 分块排序
    // TODO debug line
    std::cout << "Start divided sorting" << std::endl;
    int tempFID = 0;
    std::string dataLine;
    std::getline(stockFile, dataLine); // 读取表头
    while (std::getline(stockFile, dataLine)){
        // 读取数据到缓冲区
        // TODO debug line
        std::cout << "tempFID: " << tempFID << std::endl;
        
        stockBuffer.clear();
        stockBuffer.emplace_back(dataLine);
        for (int i = 1; i < numSortOnce; ++i){
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

    // 归并排序
    std::cout << "Start merging" << std::endl;
    MergeFilesSort(tempFID);
}


/* TEST */
void StockManager::TestReadWrite(){
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
    //TODO debug line
    std::cout << tempFID << std::endl;
    
    std::cout << "finish reading and writing test!" << std::endl;

    stockFile.close();
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

void StockManager::ShowBufferInfo(){
    std::cout << "Buffer size: " << sortMemory << std::endl;

    std::cout << "Num once: " << numSortOnce << std::endl;
}