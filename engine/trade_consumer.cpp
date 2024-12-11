#include "trade_consumer.h"
#include <iostream>
#include <csignal>
#include <matching_service.h>

bool TradeConsumer::stopFlag_ = false;

// Constructor: Initializes the Kafka consumer with provided brokers and topic
TradeConsumer::TradeConsumer(const MatchingService& service, const std::string &brokers, const std::string &topic)
    : matchingService(service), topic_(topic)
{
    // Setup Kafka consumer configuration
    const kafka::Properties props({{"bootstrap.servers",    {brokers}},
                            {"api.version.request", {"false"}},
        {"enable.auto.commit", {"false"}},
                            });
    consumerConfig_ = props;

    // Setup signal handling for graceful shutdown
    std::signal(SIGINT, stop);
}

// Start method to consume messages from Kafka
void TradeConsumer::start() {
    kafka::clients::consumer::KafkaConsumer consumer(consumerConfig_);
    consumer.subscribe({topic_});

    try {
        while (!stopFlag_) {
            // Poll messages from Kafka brokers (timeout 100ms)
            for (auto records = consumer.poll(std::chrono::milliseconds(100)); const auto& record : records) {
                if (record.error()) {
                    std::cerr << "Error: " << record.toString() << '\n';
                } else {
                    if (!records.empty())
                    {
                        std::cout << "[" << topic_ << "] Received message: "
                            << record.value().toString() << '\n';
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

// Static method to handle the stop signal
void TradeConsumer::stop(const int sig)
{
    if (sig != SIGINT) { return;
    }

    if (stopFlag_) {
        stopFlag_ = true;
    } else {
        // Restore the signal handler, -- to avoid stuck with this handler
        signal(SIGINT, SIG_IGN); // NOLINT
    }
}

// Destructor: Ensures proper cleanup when the consumer is destroyed
TradeConsumer::~TradeConsumer() {
    std::cout << "KafkaOrderBookConsumer for topic " << topic_ << " destroyed." << '\n';
}
