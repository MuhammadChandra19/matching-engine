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
    Limits Asks;
    Limits Bids;

    // Maps for fast lookup
    std::map<double, std::shared_ptr<Limit>> AskLimits;
    std::map<double, std::shared_ptr<Limit>> BidLimits;
    std::unordered_map<int64_t, std::shared_ptr<Order>> Orders;

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
        Limits sortedAsks = Asks;

        std::ranges::sort(sortedAsks, [](std::shared_ptr<Limit>& a, std::shared_ptr<Limit>& b) {
            return a->Price < b->Price;
        });

        return sortedAsks;
    }
    [[nodiscard]] Limits GetBids() const
    {
        Limits sortedBids = Bids;

        std::ranges::sort(sortedBids, [](std::shared_ptr<Limit>& a, std::shared_ptr<Limit>& b) {
            return a->Price > b->Price;
        });

        return sortedBids;
    }

private:

    void removeEmptyLimit(double price, bool isBid);
    void clearLimit(bool bid, const std::shared_ptr<Limit>& limit);
};


#endif //ORDER_BOOK_H
