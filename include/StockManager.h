#pragma once

#include "Stock.h"

class StockManager
{
private:
    int sortMemory;  // ��������õ�����ڴ�
    int numSortOnce; // ÿ�δ���Ĺ�Ʊ��
    std::vector<Stock> stockBuffer;
    std::string stockDataPath;
    std::ifstream stockFile;
    std::vector<std::string> tempFileNames;

public:
    StockManager(std::string dataPath, int maxMemory);
    // ~StockManager();

    void ReadData2Buf();                   // ��ȡһ�й�Ʊ��Ϣ������
    std::string WriteBuf2Temp(int fileID); // ��������Ϣд����ʱ�ļ��������ļ���
    void MergeFilesSort(int fileCount);    // �鲢���������ʱ�ļ�
    void ExternalSort();                   // �����ܺ���

    void TestReadWrite();
    void Openfile();
    void ShowBufferInfo();
};
