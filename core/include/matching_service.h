//
// Created by Muhammad chandra zulfikar on 10/12/24.
//

#ifndef MATCHING_SERVICE_H
#define MATCHING_SERVICE_H

#include <cstdint>
#include <vector>
#include "match.h"
#include "order_book.h"
#include "place_order_request.h"
#include "cancel_order_request.h"


struct LiveOrder {
    int64_t id;
    int64_t userId;
    double size;
    bool bid;
    int64_t timestamp;
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

        void initMatchingService();

        std::vector<Match>  handlePlaceOrder(const PlaceOrderRequest& placeOrderRequest);
        void handleCancelOrder(CancelOrderRequest request);
};

#endif //MATCHING_SERVICE_H
