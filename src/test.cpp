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
    std::string line = "a,b,2,5.3";
    T t;
    std::stringstream ss(line);
    std::getline(ss, t.A, ',');
    std::getline(ss, t.B, ',');
    std::string temp;
    std::getline(ss, temp, ',');
    t.C = std::stoi(temp);
    std::getline(ss, temp, ',');
    t.D = std::stod(temp);
    // ss >> t.B >> t.C >> t.D;
    t.Display();

    StockManager manager("../data/data_2000.csv", MAX_MEMORY);

    manager.Openfile();
    manager.TestReadWrite();

    return 0;
}