#include "../include/StockManager.h"
#include "../include/Timer.h"
#include "../include/StockPQ.h"

constexpr int MAX_MEMORY = 32 * 1024 * 1024;

// 创建索引项的结构

int main()
{
    Timer timer;

    std::cout << "START" << std::endl;
    StockManager manager("../data/China_Stock_N2.csv", MAX_MEMORY);
    
    if (0){
        manager.ExternalSort();
    }
    if (0){
        manager.GenerateIndexFile("../res/output.txt", "../res/index.txt");
    }
    if (1){
        std::cout << "Load Index..." << std::endl;

        // 加载索引文件
        auto indexMap = manager.LoadIndexFile("../res/index.txt");

        // 查询指定股票某月的数据
        std::string tsCode = "002270.SZ";
        int yearMonth = 202204;  // 年份月份
        auto stocks = manager.QueryStockData("../res/output.txt", indexMap, tsCode, yearMonth);

        std::cout << "\nResult: " << std::endl;
        if (stocks.empty())
            std::cout << "Not found!" << std::endl;

        // 打印查询结果
        for (const auto& stock : stocks) {
            stock.Display();
            std::cout << "--------------------" << std::endl;
        }
    }
    if(0){
        std::ifstream file("../data/dataForTest.txt", std::ios::in | std::ios::binary);
        if (!file.is_open()) {
            std::cerr << "Unable to open file" << std::endl;
            return 1;
        }

        // std::vector<std::streampos> lineIndex;
        // std::vector<std::pair<std::string, std::streampos>> lineIndex;
        std::unordered_map<std::string, std::streampos> lineIndex;
        std::string line;
        std::streampos pos;

        // 读取表头
        // std::getline(file, line);

        // 逐行读取文件内容，并对每行进行条件判断
        std::cout << "start..." << std::endl;
        while (true) {
            pos = file.tellg(); // 获取当前行的起始位置
            if (!std::getline(file, line)) {
                break; // 如果读取失败（包括遇到EOF），跳出循环
            }

            // 条件判断示例：这里假设条件是行包含子字符串 "keyword"
            Stock curStock(line);
            // 000993.SZ,20031219,6.35,6.47,6.18,6.44,6.36,0.08,1.26,14200.6,9027.233
            if (curStock.tsCode == "000993.SZ" && curStock.tradeDate / 100 == 200312){
                lineIndex[curStock.tsCode] = pos;
            }
            // 002253.SZ,20160129,30.72,33.29,30.55,32.87,31.14,1.73,5.56,38141.78,122774.1163
            if (curStock.tsCode == "002253.SZ" && curStock.tradeDate / 100 == 201601){
                lineIndex[curStock.tsCode] = pos;
            }
        }

        file.clear(); // 清除EOF标志
        file.seekg(0, std::ios::beg); // 重置文件位置

        // 使用索引快速访问特定行
        std::string id1 = "000993.SZ";
        std::string id2 = "002253.SZ";

        auto KV1 = lineIndex.find(id1), KV2 = lineIndex.find(id2);
        if (KV1 != lineIndex.end()){
            file.seekg(KV1->second);
            std::getline(file, line);
            Stock s1(line);
            std::cout << "Find s1: " << std::endl;
            s1.Display();
        }

        if (KV2 != lineIndex.end()){
            file.seekg(KV2->second);
            std::getline(file, line);
            Stock s2(line);
            std::cout << "Find s2: " << std::endl;
            s2.Display();
        }

        file.close();
    }
    if (0){
        std::ifstream file("../data/strTest.txt", std::ios::in | std::ios::binary);
        if (!file.is_open()) {
            std::cerr << "Unable to open file" << std::endl;
            return 1;
        }

        std::unordered_map<std::string, std::streampos> lineIndex;
        std::vector<std::streampos> vecIndex;
        std::string line;
        std::streampos pos;

        std::cout << "start test for str..." << std::endl;

        // 逐行读取文件内容，并对每行进行条件判断
        while (true) {
            pos = file.tellg(); // 获取当前行的起始位置
            if (!std::getline(file, line)) {
                break; // 如果读取失败（包括遇到EOF），跳出循环
            }

            // 条件判断示例：这里假设条件是行包含子字符串 "keyword"
            if (line.find("comfort") != std::string::npos) {
                lineIndex["comfort"] = pos; // 记录符合条件的行的位置
                vecIndex.push_back(pos);
            }
            
            if (line.find("support") != std::string::npos) {
                lineIndex["support"] = pos; // 记录符合条件的行的位置
                vecIndex.push_back(pos);
            }
        }

        file.clear(); // 清除EOF标志
        file.seekg(0, std::ios::beg); // 重置文件位置

        // 输出符合条件的行
        // 使用索引快速访问特定行
        std::string id1 = "comfort";
        std::string id2 = "support";

        auto KV1 = lineIndex.find(id1), KV2 = lineIndex.find(id2);
        if (KV1 != lineIndex.end()){
            file.seekg(KV1->second);
            std::getline(file, line);
            std::cout << "Find s1 in: \n" << line << std::endl;
        }

        if (KV2 != lineIndex.end()){
            file.seekg(KV2->second);
            std::getline(file, line);
            std::cout << "Find s2 in: \n" << line << std::endl;
        }

        file.clear(); // 清除EOF标志
        file.seekg(0, std::ios::beg); // 重置文件位置

        // 输出符合条件的行
        std::cout << "\nLines containing 'keyword':" << std::endl;
        for (const auto& index : vecIndex) {
            file.seekg(index);
            std::getline(file, line);
            std::cout << line << std::endl;
        }


        file.close();
    }


    std::cout << "Time taken: " << timer.elapsed() << " ms" << std::endl;
    
    return 0;
}