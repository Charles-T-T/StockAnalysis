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

        while (true){
            // 查询指定股票某月的数据
            std::string tsCode;
            int yearMonth;  
            std::cout << "please input tsCode(input 'quit' to quit query mood): ";
            std::cin >> tsCode;
            if (tsCode == "quit")
                break;
            std::cout << "please trade year and month: ";
            std::cin >> yearMonth;
            

            Timer queryTimer;
            auto stocks = manager.QueryStockData("../res/output.txt", indexMap, tsCode, yearMonth);

            std::cout << "\nResult: " << std::endl;
            std::cout << "--------------------" << std::endl;
            if (stocks.empty())
                std::cout << "Not found!" << std::endl;

            // 打印查询结果
            for (const auto& stock : stocks) {
                stock.Display();
                std::cout << "--------------------" << std::endl;
            }
            std::cout << "Query time taken: " << queryTimer.elapsed() << "ms\n" << std::endl;
        }
    }
    std::cout << "Whole time taken: " << timer.elapsed() << " ms" << std::endl;
    
    return 0;
}