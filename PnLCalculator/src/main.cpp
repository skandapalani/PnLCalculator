#include "PnLCalculator.hpp"
#include <iostream>

int main(int argc, char* argv[]) {
    if (argc != 3) {
        std::cerr << "Usage: " << argv[0] << " <trades.csv> <fifo|lifo>\n";
        return 1;
    }

    std::string filename = argv[1];
    bool doFifo = std::string(argv[2]) == "fifo";

    PnLCalculator calculator(doFifo);
    auto events = calculator.readTrades(filename);
    calculator.processTrades(events);

    return 0;
}