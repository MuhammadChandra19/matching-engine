//
// Created by Muhammad chandra zulfikar on 10/12/24.
//

#ifndef MATCHING_SERVICE_H
#define MATCHING_SERVICE_H

#include <cstdint>
#include <string>
#include <vector>
#include "match.h"
#include "order_book.h"


struct LiveOrder {
    int64_t id;
    int64_t userId;
    double size;
    bool bid;
    int64_t timestamp;
};

struct PlaceOrderRequest {
    int64_t id;
    int64_t userId;
    int orderType;
    bool bid;
    double size;
    double price;
    std::string pair;
};

struct  OrderBookData
{
    double totalBidVolume;
    double totalAskVolume;
    std::vector<std::shared_ptr<LiveOrder>> asks;
    std::vector<std::shared_ptr<LiveOrder>> bids;
};

class MatchingService {
    public:
        OrderBook orderBook;
        MatchingService();
        ~MatchingService();

        static void initMatchingService();

        std::vector<Match>  handlePlaceOrder(const PlaceOrderRequest& placeOrderRequest);
        void handleCancelOrder(int64_t orderID);
};

#endif //MATCHING_SERVICE_H
