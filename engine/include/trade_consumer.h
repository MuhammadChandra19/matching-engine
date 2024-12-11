//
// Created by Muhammad chandra zulfikar on 10/12/24.
//

#ifndef TRADE_CONSUMER_H
#define TRADE_CONSUMER_H

#include <kafka/KafkaConsumer.h>
#include <matching_service.h>
#include <string>

class TradeConsumer
{
public:
    MatchingService matchingService;
    // Constructor: accepts Kafka broker list and topic to subscribe to
    TradeConsumer(const MatchingService& service, const std::string &brokers, std::string topic);
    // Starts the consumer to consume messages in a loop
    void start();
    static void stop(int sig);

    // Destructor to clean up resources
    ~TradeConsumer();
private:
    static bool stopFlag_;
    std::string topic_;
    kafka::Properties consumerConfig_;  // Kafka consumer config
};

#endif //TRADE_CONSUMER_H
