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

    // ���ж�ȡ��Ʊ��Ϣ��д�뻺����
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

    // ������������д����ʱ�ļ�
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
    // ��һ�����ȶ��н��й�Ʊ����
    std::priority_queue<std::pair<Stock, int>, std::vector<std::pair<Stock, int>>, CmpPair> pq; 
    // �򿪸������鲢����ʱ�ļ�����ÿ���ļ��ĵ�һ�У�������С�ģ�д�����ȶ���pq
    for (int i = 0; i < fileCount; i++)
    {
        files[i].open("temp_" + std::to_string(i) + ".txt");
        std::string dataLine;
        std::getline(files[i], dataLine);
        pq.push(std::make_pair(Stock(dataLine), i));
    }
    // ��������ļ�
    std::string outPath = "output.txt";
    std::ofstream outFile(outPath);
    if (!outFile.is_open()){
        std::cerr << "output file open fail!" << std::endl;
    }

    int curMinFID; // ��ǰ��С�Ĺ�Ʊ��Ϣ��pq��ͷ�ģ��������ļ����
    std::string curDataLine;
    while (!pq.empty())
    {
        pq.top().first.WriteToFile(outFile);
        curMinFID = pq.top().second;
        pq.pop();
        if (std::getline(files[curMinFID], curDataLine)){
            // �Ӹ��ļ��ж�ȡ��һ�й�Ʊ����
            pq.push(std::make_pair(Stock(curDataLine), curMinFID));
        } else {
            // �����ļ��Ѷ��꣬����رղ�ɾ��
            // TODO �Ƿ���ֱ��ɾ����
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

    // �ֿ�����
    // TODO debug line
    std::cout << "Start divided sorting" << std::endl;
    int tempFID = 0;
    std::string dataLine;
    std::getline(stockFile, dataLine); // ��ȡ��ͷ
    while (std::getline(stockFile, dataLine)){
        // ��ȡ���ݵ�������
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
        // �ڻ���������
        std::sort(stockBuffer.begin(), stockBuffer.end(), CmpStock());
        // ���������ź��������д����ʱ�ļ�
        WriteBuf2Temp(tempFID++);
    }

    // �鲢����
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
   
    // ��ȡ��һ�У���ͷ��
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