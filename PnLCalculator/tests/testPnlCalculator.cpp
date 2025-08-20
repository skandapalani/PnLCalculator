#include "PnLCalculator.hpp"
#include <gtest/gtest.h>
#include <vector>

Event makeEvent(int timestamp, const std::string& symbol, char side, double price, int quantity) {
    return Event{ timestamp, symbol, side, price, quantity };
}

TEST(PnLCalculatorTest, FifoSingleTrade) {
    PnLCalculator calc(true);
    std::vector<Event> events = {
        makeEvent(1, "AAPL", 'B', 100.0, 10),
        makeEvent(2, "AAPL", 'S', 110.0, 10)
    };
    testing::internal::CaptureStdout();
    calc.processTrades(events);
    std::string output = testing::internal::GetCapturedStdout();

    std::string expected =
        "timestamp,symbol,pnl\n"
        "2,AAPL,100.00\n";
    EXPECT_EQ(output, expected);
}

TEST(PnLCalculatorTest, FifoMultipleTrades) {
    PnLCalculator calc(true);
    std::vector<Event> events = {
        makeEvent(1, "AAPL", 'B', 11.0, 15),
        makeEvent(2, "AAPL", 'B', 12.5, 15),
        makeEvent(3, "AAPL", 'S', 13.0, 20),
        makeEvent(4, "APPL", 'S', 14.0, 10),
    };
    testing::internal::CaptureStdout();
    calc.processTrades(events);
    std::string output = testing::internal::GetCapturedStdout();

    std::string expected =
        "timestamp,symbol,pnl\n"
        "3,AAPL,32.50\n";
        "4,AAPL,7.5\n";
    EXPECT_EQ(output, expected);
}

TEST(PnLCalculatorTest, LifoMultipleTrades) {
    PnLCalculator calc(false);
    std::vector<Event> events = {
        makeEvent(1, "AAPL", 'B', 11.0, 15),
        makeEvent(2, "AAPL", 'B', 12.5, 15),
        makeEvent(3, "AAPL", 'S', 13.0, 20),
        makeEvent(4, "APPL", 'S', 14.0, 10),
    };
    testing::internal::CaptureStdout();
    calc.processTrades(events);
    std::string output = testing::internal::GetCapturedStdout();

    std::string expected =
        "timestamp,symbol,pnl\n"
        "3,AAPL,17.50\n";
        "4,AAPL,22.50\n";
    EXPECT_EQ(output, expected);
}


TEST(PnLCalculatorTest, MultipleSymbolsFIFO) {
    PnLCalculator calc(true);
    std::vector<Event> events = {
        makeEvent(1, "AAPL", 'B', 100, 10),
        makeEvent(2, "GOOG", 'B', 200, 5),
        makeEvent(3, "AAPL", 'S', 110, 10),
        makeEvent(4, "GOOG", 'S', 210, 5)
    };
    testing::internal::CaptureStdout();
    calc.processTrades(events);
    std::string output = testing::internal::GetCapturedStdout();

    std::string expected =
        "timestamp,symbol,pnl\n"
        "3,AAPL,100.00\n"
        "4,GOOG,50.00\n";
    EXPECT_EQ(output, expected);
}

TEST(PnLCalculatorTest, MultipleSymbolsLIFO) {
    PnLCalculator calc(false);
    std::vector<Event> events = {
        makeEvent(1, "AAPL", 'B', 100, 10),
        makeEvent(2, "GOOG", 'B', 200, 5),
        makeEvent(3, "AAPL", 'S', 110, 10),
        makeEvent(4, "GOOG", 'S', 210, 5)
    };
    testing::internal::CaptureStdout();
    calc.processTrades(events);
    std::string output = testing::internal::GetCapturedStdout();

    std::string expected =
        "timestamp,symbol,pnl\n"
        "3,AAPL,100.00\n"
        "4,GOOG,50.00\n";
    EXPECT_EQ(output, expected);
}

TEST(PnLCalculatorTest, SellBeforeBuy) {
    PnLCalculator calc(true);
    std::vector<Event> events = {
        makeEvent(1, "AAPL", 'S', 11.0, 15),
        makeEvent(2, "AAPL", 'S', 12.5, 15),
        makeEvent(3, "AAPL", 'B', 13.0, 20),
        makeEvent(4, "APPL", 'B', 14.0, 10),
    };
    testing::internal::CaptureStdout();
    calc.processTrades(events);
    std::string output = testing::internal::GetCapturedStdout();

    std::string expected =
        "timestamp,symbol,pnl\n"
        "3,AAPL,-32.50\n";
        "4,AAPL,-7.5\n";
    EXPECT_EQ(output, expected);
}

TEST(PnLCalculatorTest, LifoSellBeforeBuy) {
    PnLCalculator calc(false);
    std::vector<Event> events = {
        makeEvent(1, "AAPL", 'S', 11.0, 15),
        makeEvent(2, "AAPL", 'S', 12.5, 15),
        makeEvent(3, "AAPL", 'B', 13.0, 20),
        makeEvent(4, "APPL", 'B', 14.0, 10),
    };
    testing::internal::CaptureStdout();
    calc.processTrades(events);
    std::string output = testing::internal::GetCapturedStdout();

    std::string expected =
        "timestamp,symbol,pnl\n"
        "3,AAPL,-17.50\n";
        "4,AAPL,-22.50\n";
    EXPECT_EQ(output, expected);
}

TEST(PnLCalculatorTest, ZeroQuantityTrade) {
    PnLCalculator calc(true);
    std::vector<Event> events = {
        makeEvent(1, "AAPL", 'B', 100.0, 0),
        makeEvent(2, "AAPL", 'S', 110.0, 0)
    };
    testing::internal::CaptureStdout();
    calc.processTrades(events);
    std::string output = testing::internal::GetCapturedStdout();

    std::string expected =
        "timestamp,symbol,pnl\n";
    EXPECT_EQ(output, expected);
}

TEST(PnLCalculatorTest, MultipleSymbolsInterleaved) {
    PnLCalculator calc(true);
    std::vector<Event> events = {
        makeEvent(1, "AAPL", 'B', 100, 10),
        makeEvent(2, "GOOG", 'B', 200, 5),
        makeEvent(3, "AAPL", 'S', 105, 5),
        makeEvent(4, "GOOG", 'S', 210, 5),
        makeEvent(5, "AAPL", 'S', 110, 5)
    };
    testing::internal::CaptureStdout();
    calc.processTrades(events);
    std::string output = testing::internal::GetCapturedStdout();

    std::string expected =
        "timestamp,symbol,pnl\n"
        "3,AAPL,25.00\n"
        "4,GOOG,50.00\n"
        "5,AAPL,50.00\n";
    EXPECT_EQ(output, expected);
}

int main(int argc, char** argv) {
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}