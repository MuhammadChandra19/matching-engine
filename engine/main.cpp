#include <matching_service.h>
#include "trade_consumer.h"


// int main(const int argc, char* argv[]) {
//     if (argc < 2) {
//         std::cerr << "Usage: " << argv[0] << " <topic>" << '\n';
//         return 1;
//     }
int main() {

    std::string const topic = "eth-btc";
    // Initialize and run the pair processor
    const auto service = std::make_unique<MatchingService>();

    // TradeConsumer tradeConsumer(service.get(), "localhost:19092", topic);
    // tradeConsumer.start();

}
