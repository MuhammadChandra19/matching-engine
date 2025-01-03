//
// Created by Muhammad chandra zulfikar on 10/12/24.
//

#ifndef ORDER_H
#define ORDER_H

#include <vector>
#include <cstdint>
#include <nlohmann/json.hpp>



class Limit;

struct BookOrder
{
    int64_t orderID;
    double size;
    bool bid;
    double price;
    int64_t userID;
    int64_t timeStamp;
    NLOHMANN_DEFINE_TYPE_INTRUSIVE(BookOrder, orderID, size, bid, price, userID, timeStamp)
};

class Order {
public:
    int64_t id;
    int64_t userID;
    double size;
    bool bid;
    std::shared_ptr<Limit> limitPtr;
    int64_t timestamp;
    int64_t sequence;

    Order() : id(0), userID(0), size(0.0), bid(false), limitPtr(nullptr), timestamp(0), sequence(0) {}
    Order(int64_t orderID, int64_t userID, double size, bool bid, std::shared_ptr<Limit> limit, int64_t timestamp);
    [[nodiscard]] bool IsFilled() const;

    int64_t NextLogSeq();
};

using Orders = std::vector<std::shared_ptr<Order>>;

#endif //ORDER_H
