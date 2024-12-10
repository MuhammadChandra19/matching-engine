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
    int64_t ID;
    int64_t UserID;
    double Size;
    bool Bid;
    std::shared_ptr<Limit> LimitPtr;
    int64_t Timestamp;

    Order() : ID(0), UserID(0), Size(0.0), Bid(false), LimitPtr(nullptr), Timestamp(0) {}
    Order(int64_t order_id, int64_t user_id, double size, bool bid, std::shared_ptr<Limit> limit, int64_t timestamp);
    [[nodiscard]] bool IsFilled() const;
};

using Orders = std::vector<std::shared_ptr<Order>>;

#endif //ORDER_H
