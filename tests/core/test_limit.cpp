#include <gtest/gtest.h>
#include <memory>
#include <limit.h>
#include <order.h>
#include <match.h>

class LimitTest : public ::testing::Test {
protected:
    LimitTest() = default;

    // Helper function to create an order
    static std::shared_ptr<Order> CreateOrder(const double size, const bool isBid) {
        auto order = std::make_shared<Order>();
        order->Size = size;
        order->Bid = isBid;
        return order;
    }
};

// Test the AddOrder method
TEST_F(LimitTest, AddOrder) {
    const auto limit = std::make_shared<Limit>(100.0); // Create a Limit with price 100.0
    const auto order1 = CreateOrder(10.0, true);  // A bid order with size 10.0
    const auto order2 = CreateOrder(5.0, false); // An ask order with size 5.0

    // Add orders to the limit
    limit->AddOrder(order1);
    limit->AddOrder(order2);

    // Check if the orders are added and the total volume is updated
    ASSERT_EQ(limit->OrderList.size(), 2);
    ASSERT_DOUBLE_EQ(limit->TotalVolume, 15.0);
}

// Test the DeleteOrder method
TEST_F(LimitTest, DeleteOrder) {
    const auto limit = std::make_shared<Limit>(100.0);
    const auto order1 = CreateOrder(10.0, true);
    const auto order2 = CreateOrder(5.0, false);

    limit->AddOrder(order1);
    limit->AddOrder(order2);

    // Remove the first order
    limit->DeleteOrder(order1);

    // Check if the order is removed and the total volume is updated
    ASSERT_EQ(limit->OrderList.size(), 1);
    ASSERT_DOUBLE_EQ(limit->TotalVolume, 5.0);
}

// Test the Fill method
TEST_F(LimitTest, FillOrders) {
    const auto limit = std::make_shared<Limit>(100.0);
    const auto order1 = CreateOrder(10.0, true);   // A bid order with size 10.0
    const auto order2 = CreateOrder(5.0, false);   // An ask order with size 5.0

    limit->AddOrder(order1);
    limit->AddOrder(order2);

    ASSERT_EQ(limit->TotalVolume, 15.0);

    // Fill the orders
    auto matches = limit->Fill(order2);  // Fill the ask order with the bid order

    // Check the match size
    ASSERT_EQ(matches.size(), 1);
    ASSERT_DOUBLE_EQ(matches[0].SizeFilled, 5.0);
    ASSERT_EQ(order2->Size, 0.0);  // The ask order should be fully filled
    ASSERT_DOUBLE_EQ(order1->Size, 5.0);  // The bid order size should be 5.0 after the fill
    ASSERT_DOUBLE_EQ(limit->TotalVolume, 10.0);  // Total volume should be updated
}

// Test if the Fill method handles partial fills
TEST_F(LimitTest, FillPartialOrder) {
    const auto limit = std::make_shared<Limit>(100.0);
    const auto order1 = CreateOrder(10.0, true);   // A bid order with size 10.0
    const auto order2 = CreateOrder(7.0, false);   // An ask order with size 7.0

    limit->AddOrder(order1);
    limit->AddOrder(order2);

    ASSERT_EQ(limit->TotalVolume, 17.0);

    // Fill the orders partially
    const auto matches = limit->Fill(order2);

    // Check if only part of the bid order is filled
    ASSERT_EQ(matches.size(), 1);
    ASSERT_DOUBLE_EQ(matches[0].SizeFilled, 7.0);  // The full size of the ask order
    ASSERT_EQ(order2->Size, 0.0);  // The ask order should be fully filled
    ASSERT_DOUBLE_EQ(order1->Size, 3.0);  // The bid order should have 3.0 remaining
    ASSERT_DOUBLE_EQ(limit->TotalVolume, 10.0);  // Total volume should be updated
}
