#pragma once
#include <string>

struct Event {
    int timestamp;
    std::string symbol;
    char side; 
    double price;
    int quantity;
};