#include "../include/StockManager.h"

#define MAX_MEMORY 32 * 1024 * 1024


class T
{
public:
    std::string A;
    std::string B;
    int C;
    double D;

    void Display()
    {
        std::cout << "A: " << A << std::endl;
        std::cout << "B: " << B << std::endl;
        std::cout << "C: " << C << std::endl;
        std::cout << "D: " << D << std::endl;
    }
};

int main()
{
    std::cout << "START" << std::endl;
    StockManager manager("../data/China_Stock_N2.csv", MAX_MEMORY);

    manager.ShowBufferInfo();

    manager.TestReadWrite();

    return 0;
}