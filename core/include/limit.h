//
// Created by Muhammad chandra zulfikar on 10/12/24.
//

#ifndef LIMIT_H
#define LIMIT_H

#include <vector>
#include "order.h"
#include "match.h"

class Limit : public std::enable_shared_from_this<Limit> {
public:
    double price;
    double totalVolume;
    std::vector<std::shared_ptr<Order>> orderList;

    explicit Limit(double price);
    void AddOrder(const std::shared_ptr<Order>& order);
    void DeleteOrder(const std::shared_ptr<Order>& order);
    std::vector<Match> Fill(const std::shared_ptr<Order>& order);

private:
    Match fillOrder(const std::shared_ptr<Order>& a_order, const std::shared_ptr<Order>& b_order) const;
};

using Limits = std::vector<std::shared_ptr<Limit>>;

#endif //LIMIT_H
