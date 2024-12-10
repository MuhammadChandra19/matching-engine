//
// Created by Muhammad chandra zulfikar on 10/12/24.
//

#include "order.h"

Order::Order(int64_t order_id,
             const int64_t user_id,
             const double size,
             const bool bid, std::shared_ptr<Limit> limit, const int64_t timestamp)
    : ID(order_id), UserID(user_id), Size(size), Bid(bid), LimitPtr(std::move(limit)), Timestamp(timestamp) {}

bool Order::IsFilled() const
{
    return Size == 0.0;
}
