#pragma once
#include "Trade.hpp"
#include "Event.hpp"
#include <string>
#include <deque>
#include <unordered_map>
#include <vector>

class PnLCalculator {
public:
    explicit PnLCalculator(bool fifo = true);

    std::vector<Event> readTrades(const std::string& filename);

    void processTrades(const std::vector<Event>& events);

private:
    bool fifo;

    std::unordered_map<std::string, std::deque<Trade>> buyPositions;
    std::unordered_map<std::string, std::deque<Trade>> sellPositions;

    double processTrade(const Event& e, std::deque<Trade>& openPositions);
};