//
// Created by Muhammad chandra zulfikar on 10/12/24.
//

#include "order.h"

Order::Order(int64_t orderID,
             const int64_t userID,
             const double size,
             const bool bid, std::shared_ptr<Limit> limit, const int64_t timestamp)
    : id(orderID), userID(userID), size(size), bid(bid), limitPtr(std::move(limit)), timestamp(timestamp) {}

bool Order::IsFilled() const
{
    return size == 0.0;
}
