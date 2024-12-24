//
// Created by Muhammad Chandra Zulfikar on 10/12/24.
//

#ifndef ORDER_BOOK_H
#define ORDER_BOOK_H

#include "limit.h"
#include "match.h"
#include <map>
#include <unordered_map>
#include <nlohmann/json.hpp>
#include <vector>

// Structure to capture a snapshot of the current order book state
struct OrderBookSnapshot {
    std::vector<BookOrder> orders; // List of all orders in the snapshot
    int64_t tradeSeq;             // Trade sequence number at the snapshot time
    int64_t logSeq;               // Log sequence number at the snapshot time

    NLOHMANN_DEFINE_TYPE_INTRUSIVE(OrderBookSnapshot, orders, tradeSeq, logSeq)
};

class OrderBook {
public:
    // Containers for ask and bid limits
    Limits asks; // Collection of all ask orders
    Limits bids; // Collection of all bid orders

    // Maps for fast lookup
    std::map<double, std::shared_ptr<Limit>> askLimits; // Map for ask price levels
    std::map<double, std::shared_ptr<Limit>> bidLimits; // Map for bid price levels
    std::unordered_map<int64_t, std::shared_ptr<Order>> Orders; // Map for order lookup by ID

    // Trade and log sequences at the current state
    int64_t tradeSeq; // Current trade sequence number
    int64_t logSeq;   // Current log sequence number

    // Constructor
    OrderBook();

    // Returns the total volume of all ask orders
    [[nodiscard]] double AskTotalVolume() const;

    // Returns the total volume of all bid orders
    [[nodiscard]] double BidTotalVolume() const;

    // Cancels an order by its ID
    void CancelOrder(int64_t orderID);

    // Places a new limit order at a specific price level
    void PlaceLimitOrder(double price, const std::shared_ptr<Order>& order);

    // Places a market order and matches it against existing orders
    std::vector<Match> PlaceMarketOrder(const std::shared_ptr<Order>& order);

    // Finds an existing limit or creates a new one at the specified price level
    std::shared_ptr<Limit> FindOrCreateLimit(double price, bool isBid);

    // Creates a snapshot of the current order book state
    [[nodiscard]] OrderBookSnapshot CreateSnapshot() const;

    // Restores the order book to a previously captured snapshot
    void Restore(const OrderBookSnapshot &snapshot);

    // Returns a sorted list of ask limits in ascending price order
    [[nodiscard]] Limits GetAsks() const
    {
        Limits sortedAsks = asks;

        std::ranges::sort(sortedAsks, [](std::shared_ptr<Limit>& a, std::shared_ptr<Limit>& b) {
            return a->price < b->price;
        });

        return sortedAsks;
    }

    // Returns a sorted list of bid limits in descending price order
    [[nodiscard]] Limits GetBids() const
    {
        Limits sortedBids = bids;

        std::ranges::sort(sortedBids, [](std::shared_ptr<Limit>& a, std::shared_ptr<Limit>& b) {
            return a->price > b->price;
        });

        return sortedBids;
    }

private:
    // Removes a limit if it becomes empty
    void removeEmptyLimit(double price, bool isBid);

    // Clears all orders within a specific limit
    void clearLimit(bool bid, const std::shared_ptr<Limit>& limit);
};

#endif //ORDER_BOOK_H
