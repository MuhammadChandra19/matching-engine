//
// Created by Muhammad chandra zulfikar on 10/12/24.
//

#ifndef MATCH_H
#define MATCH_H

#include "order.h"

class Match {
public:
    std::shared_ptr<Order> Ask;
    std::shared_ptr<Order> Bid;
    double SizeFilled;
    double Price;

    Match(std::shared_ptr<Order> ask, std::shared_ptr<Order> bid, double size_filled, double price);
};


#endif //MATCH_H
