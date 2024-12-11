#include <matching_service.h>
#include "trade_consumer.h"
#include <iostream>


// int main(const int argc, char* argv[]) {
//     if (argc < 2) {
//         std::cerr << "Usage: " << argv[0] << " <topic>" << '\n';
//         return 1;
//     }
int main(const int argc, char* argv[]) {

    std::string const topic = "eth-btc";
    // Initialize and run the pair processor
    MatchingService const service;

    TradeConsumer tradeConsumer(service, "localhost:19092", topic);
    tradeConsumer.start();

}
