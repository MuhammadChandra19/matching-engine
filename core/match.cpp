//
// Created by Muhammad chandra zulfikar on 10/12/24.
//

#include "match.h"

Match::Match(std::shared_ptr<Order> ask, std::shared_ptr<Order> bid, const double sizeFilled, const double price)
    : ask(std::move(ask)), bid(std::move(bid)), sizeFilled(sizeFilled), Price(price) {}
