#include "Stock.h"

Stock::Stock() : tsCode(""), tradeDate(0), open(0), high(0), low(0), close(0), preClose(0), change(0), pctChg(0), vol(0), amount(0) {}

Stock::Stock(std::string dataLine){
    std::stringstream ss(dataLine);
    
}
