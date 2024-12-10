//
// Created by Muhammad chandra zulfikar on 10/12/24.
//

#ifndef ORDER_H
#define ORDER_H

#include <memory>
#include <vector>
#include <cstdint>

class Limit;

class Order {
public:
    int64_t id;
    int64_t userID;
    double size;
    bool bid;
    std::shared_ptr<Limit> limitPtr;
    int64_t timestamp;

    Order() : id(0), userID(0), size(0.0), bid(false), limitPtr(nullptr), timestamp(0) {}
    Order(int64_t orderID, int64_t userID, double size, bool bid, std::shared_ptr<Limit> limit, int64_t timestamp);
    [[nodiscard]] bool IsFilled() const;
};

using Orders = std::vector<std::shared_ptr<Order>>;

#endif //ORDER_H
