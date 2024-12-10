//
// Created by Muhammad chandra zulfikar on 10/12/24.
//
#include "limit.h"
#include <algorithm>

Limit::Limit(const double price) : Price(price), TotalVolume(0.0) {}

void Limit::AddOrder(const std::shared_ptr<Order>& order) {
    OrderList.push_back(order);
    TotalVolume += order->Size;
    // Ensure the order's LimitPtr points back to this Limit instance
    order->LimitPtr = shared_from_this();
}

void Limit::DeleteOrder(const std::shared_ptr<Order>& order) {
    if (const auto shared_ptr = std::ranges::find(OrderList, order); shared_ptr != OrderList.end()) {
        // Replace the found order with the last order
        *shared_ptr = OrderList.back();
        OrderList.pop_back();  // Remove the last order
    }

    // Set the limit reference of the order to nullptr
    order->LimitPtr = nullptr;
    TotalVolume -= order->Size;

    std::ranges::sort(OrderList, [](const std::shared_ptr<Order>& lhs, const std::shared_ptr<Order>& rhs) {
        return lhs->Timestamp < rhs->Timestamp;
    });

}

std::vector<Match> Limit::Fill(const std::shared_ptr<Order>& order) {
    std::vector<Match> matches;
    std::vector<std::shared_ptr<Order>> ordersToDelete;

    for (const auto& orderPtr : OrderList)
    {
        Match const match = fillOrder(orderPtr, order);
        matches.push_back(match);

        TotalVolume -= match.SizeFilled;

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
        DeleteOrder(orderPtr);
    }


    return matches;
}

Match Limit::fillOrder(const std::shared_ptr<Order>& a_order, const std::shared_ptr<Order>& b_order) const
{
    std::shared_ptr<Order> bid;
    std::shared_ptr<Order> ask;
    double sizeFilled = 0.0;

    // Determine bid and ask orders
    if (a_order->Bid) {
        bid = a_order;
        ask = b_order;
    } else {
        bid = b_order;
        ask = a_order;
    }

    // Adjust sizes and determine the size filled
    if (a_order->Size > b_order->Size) {
        a_order->Size -= b_order->Size;
        sizeFilled = b_order->Size;
        b_order->Size = 0.0;
    } else {
        b_order->Size -= a_order->Size;
        sizeFilled = a_order->Size;
        a_order->Size = 0.0;
    }

    // Return the match result
    return Match{
        ask,           // Bid order
        bid,           // Ask order
        sizeFilled,    // Size filled
        Price          // Price of the limit
    };
}
