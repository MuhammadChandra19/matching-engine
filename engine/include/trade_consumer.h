//
// Created by Muhammad chandra zulfikar on 10/12/24.
//

#ifndef TRADE_CONSUMER_H
#define TRADE_CONSUMER_H

#include <kafka/KafkaConsumer.h>
#include <matching_service.h>
#include <string>

class TradeConsumer {
public:
    // Constructor: accepts Kafka broker list and topic to subscribe to
    TradeConsumer(MatchingService *service, const std::string &brokers, std::string topic, int64_t lastOffset, const bool &stopFlag);
    // Starts the consumer to consume messages in a loop
    void start();

    // Destructor to clean up resources
    ~TradeConsumer();
private:
    bool stopFlag;
    MatchingService* service;
    std::string topic_;
    kafka::Properties consumerConfig_;  // Kafka consumer config
    int64_t lastOffset_;
};

#endif //TRADE_CONSUMER_H
