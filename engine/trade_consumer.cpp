#include "trade_consumer.h"
#include <cancel_order_request.h>
#include <iostream>
#include <matching_service.h>
#include <place_order_request.h>
#include <string_helper.h>

// Constructor: Initializes the Kafka consumer with provided brokers and topic
TradeConsumer::TradeConsumer(MatchingService *service, const std::string &brokers, std::string topic,
                             const int64_t lastOffset,
                             const bool &stopFlag )
    : service(service), topic_(std::move(topic)), lastOffset_(lastOffset), stopFlag(stopFlag)
{


    // Setup Kafka consumer configuration
    const kafka::Properties props({
        {"bootstrap.servers",    {brokers}},
        {"api.version.request", {"false"}},
        {"enable.auto.commit", {"false"}},
    });
    consumerConfig_ = props;
}

// Start method to consume messages from Kafka
void TradeConsumer::start() {
    kafka::clients::consumer::KafkaConsumer consumer(consumerConfig_);
    consumer.subscribe({topic_});

    if (lastOffset_ > 0)
    {
        kafka::TopicPartition const topicPartition(topic_, 0);
        consumer.seek(topicPartition, lastOffset_);
    }

    try {
        while (!stopFlag) {
            // Poll messages from Kafka brokers (timeout 100ms)
            for (auto records = consumer.poll(std::chrono::milliseconds(100)); const auto& record : records) {
                if (record.error()) {
                    std::cerr << "Error: " << record.toString() << '\n';
                } else {
                    if (!records.empty())
                    {
                        std::string const message = record.value().toString();
                        std::cout << message << '\n';
                        if (auto kvPairs = StringHelper::parseKeyValue(message); kvPairs["type"] == "place_order")
                        {
                            PlaceOrderRequest const request = parsePlaceOrder(kvPairs);
                            service->handlePlaceOrder(request);


                        } else if (kvPairs["type"] == "cancel_order")
                        {
                            CancelOrderRequest const cancelOrder = parseCancelOrder(kvPairs);
                            service->handleCancelOrder(cancelOrder);
                        } else
                        {
                            std::cerr << "Unknown message type: " << kvPairs["type"] << '\n';
                        }
                        consumer.commitAsync();
                    }
                }
            }
        }

        // Close the consumer gracefully when the loop ends
        consumer.commitSync();
    } catch (const kafka::KafkaException& e) {
        std::cerr << "Kafka exception: " << e.what() << '\n';
    }
}

// Destructor: Ensures proper cleanup when the consumer is destroyed
TradeConsumer::~TradeConsumer() {
    std::cout << "KafkaOrderBookConsumer for topic " << topic_ << " destroyed." << '\n';
}
