#include <gtest/gtest.h>
#include <memory>

#include "order.h"
#include "order_book.h"
#include "limit.h"
#include "match.h"

class TestOrderBook : public ::testing::Test {
protected:
    TestOrderBook() {}

    // Helper function to create an order
    static std::shared_ptr<Order> CreateOrder(int64_t id, int64_t user_id, double size, bool isBid, std::shared_ptr<Limit> limit, int64_t timestamp) {
        return std::make_shared<Order>(id, user_id, size, isBid, limit, timestamp);
    }

    // Helper function to create a Limit and Place an order
    static void PlaceOrder(OrderBook& orderbook,
                    const double price,
                    const double size,
                    const bool isBid, const std::string& id,
                    const int64_t user_id,
                    const int64_t timestamp) {
        const auto limit = orderbook.FindOrCreateLimit(price, isBid);
        const auto order = CreateOrder(std::stoi(id), user_id, size, isBid, limit, timestamp);
        orderbook.PlaceLimitOrder(price, order);
    }
};

// Test AskTotalVolume method
TEST_F(TestOrderBook, AskTotalVolume) {
    OrderBook orderbook;

    // Place some ask orders
    PlaceOrder(orderbook, 100.0, 10.0, false, "1", 12345, 1678901234);
    PlaceOrder(orderbook, 101.0, 20.0, false, "2", 12346, 1678901244);

    // Check total ask volume
    ASSERT_DOUBLE_EQ(orderbook.AskTotalVolume(), 30.0);
}

// Test BidTotalVolume method
TEST_F(TestOrderBook, BidTotalVolume) {
    OrderBook orderbook;

    // Place some bid orders
    PlaceOrder(orderbook, 99.0, 15.0, true, "1", 12345, 1678901234);
    PlaceOrder(orderbook, 98.0, 25.0, true, "2", 12346, 1678901244);

    // Check total bid volume
    ASSERT_DOUBLE_EQ(orderbook.BidTotalVolume(), 40.0);
}

// Test CancelOrder method
TEST_F(TestOrderBook, CancelOrder) {
    OrderBook orderbook;

    // Place an order
    PlaceOrder(orderbook, 100.0, 10.0, false, "1", 12345, 1678901234);
    PlaceOrder(orderbook, 100.0, 5.0, false, "2", 12346, 1678901244);

    const auto order = CreateOrder(3, 12347, 5.0, false, nullptr, 1678901254);
    orderbook.CancelOrder(order);  // Trying to cancel an order that hasn't been placed

    // Check if the order is removed
    ASSERT_EQ(orderbook.Orders.size(), 2);  // The orderbook should have 2 orders still placed
}

// Test PlaceLimitOrder method
TEST_F(TestOrderBook, PlaceLimitOrder) {
    OrderBook orderbook;

    // Place a limit order
    PlaceOrder(orderbook, 100.0, 10.0, false, "1", 12345, 1678901234);

    // Check if the limit has been added
    auto limit = orderbook.Asks.front();
    ASSERT_EQ(limit->TotalVolume, 10.0);
    ASSERT_EQ(limit->OrderList.size(), 1);
}

// Test PlaceMarketOrder method
TEST_F(TestOrderBook, PlaceMarketOrder) {
    OrderBook orderbook;

    // Place bid and ask orders
    PlaceOrder(orderbook, 100.0, 10.0, false, "1", 12345, 1678901234);
    PlaceOrder(orderbook, 99.0, 5.0, false, "2", 12346, 1678901244);

    // Place a market order
    const auto marketOrder = CreateOrder(4, 12347, 15.0, true, nullptr, 1678901254);
    const auto matches = orderbook.PlaceMarketOrder(marketOrder);

    // Verify that the market order was filled
    ASSERT_EQ(matches.size(), 2);
    ASSERT_DOUBLE_EQ(matches[0].SizeFilled, 5.0);
    ASSERT_EQ(marketOrder->Size, 0.0);  // The market order should be fully filled
}

// Test clearLimit method (removes empty limits)
TEST_F(TestOrderBook, ClearLimit) {
    OrderBook orderbook;

    // Place some orders
    PlaceOrder(orderbook, 100.0, 10.0, false, "1", 12345, 1678901234);
    PlaceOrder(orderbook, 99.0, 5.0, false, "2", 12346, 1678901244);

    // Cancel the bid order
    auto bidOrder = CreateOrder(2, 12347, 5.0, true, nullptr, 1678901254);
    orderbook.CancelOrder(bidOrder);

    // Verify that the empty limit is cleared
    ASSERT_TRUE(orderbook.BidLimits.empty());  // Bid limit should be removed
}

// Test FindOrCreateLimit method
TEST_F(TestOrderBook, FindOrCreateLimit) {
    OrderBook orderbook;

    // Create a limit order
    PlaceOrder(orderbook, 100.0, 10.0, false, "1", 12345, 1678901234);

    // Find or create limit for the same price
    const auto limit = orderbook.FindOrCreateLimit(100.0, false);

    // Verify the limit is the same
    ASSERT_NE(limit, nullptr);
    ASSERT_EQ(limit->Price, 100.0);

    // Create another limit for a different price
    auto newLimit = orderbook.FindOrCreateLimit(101.0, false);
    ASSERT_NE(newLimit, nullptr);
    ASSERT_EQ(newLimit->Price, 101.0);
}
