//
// Created by Muhammad chandra zulfikar on 10/12/24.
//
#include "limit.h"
#include <algorithm>
#include "log.h"

Limit::Limit(const double price) : price(price), totalVolume(0.0) {}

Log* Limit::AddOrder(const std::shared_ptr<Order>& order) {
    orderList.push_back(order);
    totalVolume += order->size;
    // Ensure the order's LimitPtr points back to this Limit instance
    order->limitPtr = shared_from_this();

    auto *const log = new OpenLog(
            order->NextLogSeq(),
            "PAIR",
            order->id,
            order->size,
            order->limitPtr->price,
            order->bid
        );
    return log;
}

std::unique_ptr<Log> Limit::DeleteOrder(const std::shared_ptr<Order>& order) {
    if (const auto shared_ptr = std::ranges::find(orderList, order); shared_ptr != orderList.end()) {
        // Replace the found order with the last order
        *shared_ptr = orderList.back();
        orderList.pop_back();  // Remove the last order
    }
    auto log = std::make_unique<DoneLog>(
        order->NextLogSeq(),
        "PAIR",
        order->id,
        order->size,
        order->limitPtr->price,
        "Reason",
        order->bid
    );

    // Set the limit reference of the order to nullptr
    order->limitPtr = nullptr;
    totalVolume -= order->size;

    std::ranges::sort(orderList, [](const std::shared_ptr<Order>& lhs, const std::shared_ptr<Order>& rhs) {
        return lhs->timestamp < rhs->timestamp;
    });
    return log;
}

FillResult Limit::Fill(const std::shared_ptr<Order>& order) {
    std::vector<Match> matches;
    std::vector<std::shared_ptr<Order>> ordersToDelete;
    std::vector<std::unique_ptr<Log>> logs;

    for (const auto& orderPtr : orderList)
    {
        Match const match = fillOrder(orderPtr, order);
        matches.push_back(match);

        totalVolume -= match.sizeFilled;

        std::unique_ptr<Log> const log = std::make_unique<MatchLog> (
            orderPtr->NextLogSeq(),
            "PAIR",
            order->id,
            orderPtr->id,
            orderPtr->limitPtr->price,
            orderPtr->size);
        logs.push_back(log);

        if (orderPtr->IsFilled())
        {
            ordersToDelete.push_back(orderPtr);
        }

        if (order->IsFilled())
        {
            break;
        }
    }

    for (const auto& orderPtr : ordersToDelete)
    {
        auto deleteLog = DeleteOrder(orderPtr);
        logs.push_back(deleteLog);
    }


    return FillResult{.matches=matches, .logs=logs};
}

Match Limit::fillOrder(const std::shared_ptr<Order>& a_order, const std::shared_ptr<Order>& b_order) const
{
    std::shared_ptr<Order> bid;
    std::shared_ptr<Order> ask;
    double sizeFilled = 0.0;

    // Determine bid and ask orders
    if (a_order->bid) {
        bid = a_order;
        ask = b_order;
    } else {
        bid = b_order;
        ask = a_order;
    }

    // Adjust sizes and determine the size filled
    if (a_order->size > b_order->size) {
        a_order->size -= b_order->size;
        sizeFilled = b_order->size;
        b_order->size = 0.0;
    } else {
        b_order->size -= a_order->size;
        sizeFilled = a_order->size;
        a_order->size = 0.0;
    }

    // Return the match result
    return Match{
        ask,           // Bid order
        bid,           // Ask order
        sizeFilled,    // Size filled
        price          // Price of the limit
    };
}
