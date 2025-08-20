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
        Event event;
        std::getline(ss, token, ','); 
        event.timestamp = std::stoi(token);
        std::getline(ss, event.symbol, ',');
        std::getline(ss, token, ','); 
        event.side = token[0];
        std::getline(ss, token, ','); 
        event.price = std::stod(token);
        std::getline(ss, token, ','); 
        event.quantity = std::stoi(token);
        events.push_back(event);
    }
    return events;
}

double PnLCalculator::processTrade(const Event& event, std::deque<Trade>& openPositions) {
    double pnl = 0.0;
    int remaining = event.quantity;

    while (remaining > 0) {
        if (openPositions.empty())
        {
            break;
        }
        Trade& op = fifo ? openPositions.front() : openPositions.back();
        int matchedQty = std::min(remaining, op.quantity);

        if (event.side == 'S')
        {
            pnl += matchedQty * (event.price - op.price);
        }
        else
        {
            pnl += matchedQty * (op.price - event.price);
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
    for (const auto& event : events) {
        double pnl = 0.0;
        auto& openPos = (event.side == 'S') ? buyPositions[event.symbol] : sellPositions[event.symbol];
        pnl = processTrade(event, openPos);

        if (pnl != 0.0)
        {
            std::cout << event.timestamp << "," << event.symbol << "," << std::fixed << std::setprecision(2) << pnl << "\n";
        }
            
        auto& storePos = (event.side == 'B') ? buyPositions[event.symbol] : sellPositions[event.symbol];
        if (event.quantity > 0)
        {
            storePos.push_back({ event.price, event.quantity });
        }
    }
}



