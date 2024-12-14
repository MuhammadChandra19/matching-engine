//
// Created by Muhammad chandra zulfikar on 10/12/24.
//

#include "order_book.h"
#include <algorithm>
#include <numeric>

OrderBook::OrderBook() = default;

// Calculate total volume of asks
double OrderBook::AskTotalVolume() const {
    return std::accumulate(asks.begin(), asks.end(), 0.0,
        [](const double sum, const std::shared_ptr<Limit>& limit) {
            return sum + limit->totalVolume;
        });
}

// Calculate total volume of bids
double OrderBook::BidTotalVolume() const {
    return std::accumulate(bids.begin(), bids.end(), 0.0,
        [](const double sum, const std::shared_ptr<Limit>& limit) {
            return sum + limit->totalVolume;
        });
}

// Cancel an existing order
void OrderBook::CancelOrder(const int64_t orderID) {
    const auto match_order = Orders.find(orderID);
    if (match_order == Orders.end()) {
        return; // Order not found
    }

    const auto& order = match_order->second;
    if (const auto limit = order->limitPtr) {
        limit->DeleteOrder(order);

        // Remove empty limits
        if (limit->orderList.empty()) {
            clearLimit(order->bid, limit);
        }
    }

    Orders.erase(match_order);
}

// Place a limit order
void OrderBook::PlaceLimitOrder(const double price, const std::shared_ptr<Order>& order) {
    const auto limit = FindOrCreateLimit(price, order->bid);
    order->limitPtr = limit;
    limit->AddOrder(order);
    Orders[order->id] = order;
}

// Place a market order and match it with existing orders
std::vector<Match> OrderBook::PlaceMarketOrder(const std::shared_ptr<Order>& order) {
    std::vector<Match> matches;
    const auto & limits = order->bid ? GetAsks() : GetBids(); // Select the opposite side of the orderbook

    if (order->bid)
    {
        if (order->size > AskTotalVolume())
        {
            // not enough volume
            return matches;
        }
    } else
    {
        if (order->size > BidTotalVolume())
        {
            // not enough volume
            return matches;
        }
    }

    for (const auto& limit : limits)
    {
        std::vector<Match> limit_matches = limit->Fill(order);
        matches.insert(matches.end(), limit_matches.begin(), limit_matches.end());
        if (limit->orderList.empty())
        {
            clearLimit(order->bid, limit);
        }
    }

    return matches;
}

// Clear an empty limit from the orderbook
void OrderBook::clearLimit(const bool bid, const std::shared_ptr<Limit>& limit) {
    auto& limitMap = bid ? bidLimits : askLimits;
    auto& limitsList = bid ? bids : asks;

    limitMap.erase(limit->price);
    std::erase_if(limitsList, [&limit](const auto &lim) { return lim->price == limit->price; });
}

// Find or create a new limit for a given price
std::shared_ptr<Limit> OrderBook::FindOrCreateLimit(double price, const bool isBid) {
    auto& limits = isBid ? bids : asks;
    auto& limitMap = isBid ? bidLimits : askLimits;

    if (const auto match_limit = limitMap.find(price); match_limit != limitMap.end()) {
        return match_limit->second;
    }

    auto newLimit = std::make_shared<Limit>(price);
    limits.push_back(newLimit);
    limitMap[price] = newLimit;
    return newLimit;
}

// Remove an empty limit if it exists
void OrderBook::removeEmptyLimit(const double price, const bool isBid) {
    auto& limitMap = isBid ? bidLimits : askLimits;

    if (const auto match_limit = limitMap.find(price);
        match_limit != limitMap.end() && match_limit->second->orderList.empty()) {
        limitMap.erase(match_limit);
    }
}

OrderBookSnapshot OrderBook::CreateSnapshot() const
{
    OrderBookSnapshot snapshot;
    for (const auto &[fst, snd] : Orders)
    {
        snapshot.orders.push_back(BookOrder{
            .orderID = fst,
            .size = snd->size,
            .bid = snd->bid,
            .price = snd->limitPtr->price,
        });

    }
    snapshot.logSeq = logSeq;
    snapshot.tradeSeq = tradeSeq;
    return  snapshot;
}

void OrderBook::Restore(const OrderBookSnapshot &snapshot)
{
    for (const auto& snapshotOrder : snapshot.orders)
    {
        auto order = std::make_shared<Order>(
            snapshotOrder.orderID,
            snapshotOrder.price,
            snapshotOrder.size,
            snapshotOrder.bid,
            nullptr,
            snapshotOrder.timeStamp
            );
        PlaceLimitOrder(snapshotOrder.orderID, order);
    }
}

