//
// Created by Muhammad chandra zulfikar on 10/12/24.
//

#ifndef ORDER_BOOK_H
#define ORDER_BOOK_H

#include "limit.h"
#include "match.h"
#include <map>
#include <unordered_map>
#include <memory>
#include <vector>

class OrderBook {
public:
    // Containers for ask and bid limits
    Limits asks;
    Limits bids;

    // Maps for fast lookup
    std::map<double, std::shared_ptr<Limit>> askLimits;
    std::map<double, std::shared_ptr<Limit>> bidLimits;
    std::unordered_map<int64_t, std::shared_ptr<Order>> Orders;

    int64_t seq;
    int64_t logOffset;
    int64_t logSeq;

    OrderBook();

    // Core methods
    [[nodiscard]] double AskTotalVolume() const;
    [[nodiscard]] double BidTotalVolume() const;
    void CancelOrder(int64_t orderID);
    void PlaceLimitOrder(double price, const std::shared_ptr<Order>& order);
    std::vector<Match> PlaceMarketOrder(const std::shared_ptr<Order>& order);
    std::shared_ptr<Limit> FindOrCreateLimit(double price, bool isBid);


    // Getters for sorted limits
    [[nodiscard]] Limits GetAsks() const
    {
        Limits sortedAsks = asks;

        std::ranges::sort(sortedAsks, [](std::shared_ptr<Limit>& a, std::shared_ptr<Limit>& b) {
            return a->price < b->price;
        });

        return sortedAsks;
    }
    [[nodiscard]] Limits GetBids() const
    {
        Limits sortedBids = bids;

        std::ranges::sort(sortedBids, [](std::shared_ptr<Limit>& a, std::shared_ptr<Limit>& b) {
            return a->price > b->price;
        });

        return sortedBids;
    }

private:

    void removeEmptyLimit(double price, bool isBid);
    void clearLimit(bool bid, const std::shared_ptr<Limit>& limit);
};


#endif //ORDER_BOOK_H
