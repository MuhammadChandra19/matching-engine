//
// Created by Muhammad chandra zulfikar on 10/12/24.
//
#include "trade_consumer.h"
#include <iostream>
#include <csignal>


bool TradeConsumer::stopFlag_ = false;
TradeConsumer::TradeConsumer(const std::string &brokers, const std::string &topic): topic_(topic)
{
    const kafka::Properties props({
            {"bootstrap.servers", brokers},
        {"group.id", "orderbook-consumer-group-" + topic}
    });

    consumerConfig_ = props;
    std::signal(SIGINT, [](int signal) { stopFlag_ = true; });
}

void TradeConsumer::start(){
    kafka::clients::consumer::KafkaConsumer consumer(consumerConfig_);
    consumer.subscribe({ topic_ });
    std::cout << "Consumer subscribed to topic: " << topic_ << '\n';
    try{
        while (!stopFlag_)
        {
            for (auto records = consumer.poll(std::chrono::milliseconds(100)); const auto& record : records) {
                if (record.error()) {
                    std::cerr << "Error: " << record.toString() << '\n';
                } else {
                    std::cout << "[" << topic_ << "] Received message: "
                              << record.value().toString() << '\n';
                }
            }
        }
        consumer.close();
    } catch (const kafka::KafkaException& e ){
        consumer.close();
        std::cerr << "Kafka exception: " << e.what() << '\n';
    }
}

 TradeConsumer::~TradeConsumer()
{
    std::cout << "KafkaOrderBookConsumer for topic " << topic_ << " destroyed." << '\n';
}

