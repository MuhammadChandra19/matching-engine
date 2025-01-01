//
// Created by Muhammad chandra zulfikar on 10/12/24.
//

#ifndef LIMIT_H
#define LIMIT_H

#include "match.h"
#include "order.h"
#include <vector>

#include "log.h"

struct FillResult
{
    std::vector<Match> matches;
    std::vector<std::unique_ptr<Log>> logs;
};

class Limit : public std::enable_shared_from_this<Limit> {
public:
    double price;
    double totalVolume;
    std::vector<std::shared_ptr<Order>> orderList;

    explicit Limit(double price);
    Log* AddOrder(const std::shared_ptr<Order>& order);
    std::unique_ptr<Log> DeleteOrder(const std::shared_ptr<Order>& order);
    FillResult Fill(const std::shared_ptr<Order>& order);

private:
    Match fillOrder(const std::shared_ptr<Order>& a_order, const std::shared_ptr<Order>& b_order) const;
};

using Limits = std::vector<std::shared_ptr<Limit>>;

#endif //LIMIT_H
