//
// Created by Muhammad chandra zulfikar on 10/12/24.
//

#include "match.h"

Match::Match(std::shared_ptr<Order> ask, std::shared_ptr<Order> bid, const double size_filled, const double price)
    : Ask(std::move(ask)), Bid(std::move(bid)), SizeFilled(size_filled), Price(price) {}
