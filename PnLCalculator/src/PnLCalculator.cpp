#include "PnLCalculator.hpp"
#include <fstream>
#include <sstream>
#include <iostream>
#include <iomanip>
#include <algorithm>

PnLCalculator::PnLCalculator(bool doFifo) : fifo(doFifo) {}

std::vector<Event> PnLCalculator::readTrades(const std::string& filename) {
    std::vector<Event> events;
    std::ifstream infile(filename);
    std::string line;
    while (std::getline(infile, line)) {
        std::stringstream ss(line);
        std::string token;
        Event e;
        std::getline(ss, token, ','); 
        e.timestamp = std::stoi(token);
        std::getline(ss, e.symbol, ',');
        std::getline(ss, token, ','); 
        e.side = token[0];
        std::getline(ss, token, ','); 
        e.price = std::stod(token);
        std::getline(ss, token, ','); 
        e.quantity = std::stoi(token);
        events.push_back(e);
    }
    return events;
}

double PnLCalculator::processTrade(const Event& e, std::deque<Trade>& openPositions) {
    double pnl = 0.0;
    int remaining = e.quantity;

    while (remaining > 0) {
        if (openPositions.empty())
        {
            break;
        }
        Trade& op = fifo ? openPositions.front() : openPositions.back();
        int matchedQty = std::min(remaining, op.quantity);

        if (e.side == 'S')
        {
            pnl += matchedQty * (e.price - op.price);
        }
        else
        {
            pnl += matchedQty * (op.price - e.price);
        }

        remaining -= matchedQty;
        op.quantity -= matchedQty;

        if (op.quantity == 0)
        {
            fifo ? openPositions.pop_front() : openPositions.pop_back();
        }
    }

    return pnl;
}

void PnLCalculator::processTrades(const std::vector<Event>& events) {
    std::cout << "timestamp,symbol,pnl\n";
    for (const auto& e : events) {
        double pnl = 0.0;
        auto& openPos = (e.side == 'S') ? buyPositions[e.symbol] : sellPositions[e.symbol];
        pnl = processTrade(e, openPos);

        if (pnl != 0.0)
        {
            std::cout << e.timestamp << "," << e.symbol << "," << std::fixed << std::setprecision(2) << pnl << "\n";
        }
            
        auto& storePos = (e.side == 'B') ? buyPositions[e.symbol] : sellPositions[e.symbol];
        if (e.quantity > 0)
        {
            storePos.push_back({ e.price, e.quantity });
        }
    }
}



