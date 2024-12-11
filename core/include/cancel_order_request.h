//
// Created by Muhammad chandra zulfikar on 11/12/24.
//

#ifndef CANCEL_ORDER_REQUEST_H
#define CANCEL_ORDER_REQUEST_H
#include <string>
struct CancelOrderRequest {
    int64_t orderId;
};

// Specialization for CancelOrderRequest in this file
inline CancelOrderRequest parseCancelOrder(const std::map<std::string, std::string>& kvPairs) {
    CancelOrderRequest request{};
    request.orderId = std::stoll(kvPairs.at("orderId"));
    return request;
}

#endif //CANCEL_ORDER_REQUEST_H
