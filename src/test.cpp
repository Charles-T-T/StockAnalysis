#include "StockManager.h"
#include "Timer.h"

constexpr int MAX_MEMORY = 32 * 1024 * 1024;

int main()
{
    Timer timer;

    std::cout << "START" << std::endl;
    
    StockManager manager("../data/China_Stock_N2.csv", MAX_MEMORY);

    manager.ExternalSort();

    std::cout << "Time taken: " << timer.elapsed() << " ms" << std::endl;
    
    return 0;
}