//
// Created by Muhammad chandra zulfikar on 11/12/24.
//

#ifndef PLACE_ORDER_REQUEST_H
#define PLACE_ORDER_REQUEST_H
#include <string>
#include <nlohmann/json.hpp>

struct PlaceOrderRequest
{
    int64_t id;
    int64_t userId;
    int orderType;
    bool bid;
    double size;
    double price;
    std::string pair;
    std::string createdAt;
    int64_t offset;

    NLOHMANN_DEFINE_TYPE_INTRUSIVE(PlaceOrderRequest, id, userId, orderType, bid, size, price, pair, createdAt, offset);

};

inline PlaceOrderRequest parsePlaceOrder(const std::map<std::string, std::string>& kvPairs) {
    PlaceOrderRequest order;
    order.id = std::stoll(kvPairs.at("id"));
    order.userId = std::stoll(kvPairs.at("userId"));
    order.orderType = std::stoi(kvPairs.at("orderType"));
    order.bid = kvPairs.at("bid") == "true";
    order.size = std::stod(kvPairs.at("size"));
    order.price = std::stod(kvPairs.at("price"));
    order.pair = kvPairs.at("pair");
    return order;
}




#endif //PLACE_ORDER_REQUEST_H
