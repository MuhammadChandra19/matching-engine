//
// Created by Muhammad chandra zulfikar on 29/12/24.
//

#ifndef ORDER_READER_H
#define ORDER_READER_H
#include <iostream>
#include <kafka/KafkaConsumer.h>
#include <place_order_request.h>
#include <string>

class OrderReader {
public:
    OrderReader(
        const std::string &brokers,
        const std::string &topic,
        int64_t lastOffset,
        const bool &stopFlag
    );

    void setOffset(int64_t offset);
    PlaceOrderRequest fetchOrder();

    ~OrderReader();
private:
    bool stopFlag{};
    std::string topic_;
    kafka::clients::consumer::KafkaConsumer consumer_;
    int64_t lastOffset_{};
};

#endif //ORDER_READER_H
