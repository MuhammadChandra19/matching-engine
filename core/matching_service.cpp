//
// Created by Muhammad chandra zulfikar on 10/12/24.
//
#include <iostream>
#include <vector>
#include "matching_service.h"
#include "order.h"

MatchingService::~MatchingService() {
    std::cout << "MatchingService Destroyed." << '\n';
}
MatchingService::MatchingService() {
    orderBook = std::make_unique<OrderBook>();
    std::cout << "MatchingService Constructor." << '\n';
}

std::vector<Match> MatchingService::handlePlaceOrder(const PlaceOrderRequest& placeOrderRequest) const
{
    std::vector<Match> matches;
    const auto limit = orderBook->FindOrCreateLimit(placeOrderRequest.price, placeOrderRequest.bid);
    auto const order = std::make_shared<Order>(
        placeOrderRequest.id,
        placeOrderRequest.userId,
        placeOrderRequest.size,
        placeOrderRequest.bid,
        limit,
        std::chrono::duration_cast<std::chrono::milliseconds>(
                                    std::chrono::system_clock::now().time_since_epoch())
                                    .count()
    );

    if (placeOrderRequest.orderType == 0) {
        orderBook->PlaceLimitOrder(placeOrderRequest.price, order);
    }

    if (placeOrderRequest.orderType == 1) {
        matches = orderBook->PlaceMarketOrder(order);
    }
    return matches;
}

void MatchingService::handleCancelOrder(const CancelOrderRequest request) const {
    orderBook->CancelOrder(request.orderId);
}
