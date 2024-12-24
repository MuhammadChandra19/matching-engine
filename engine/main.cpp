#include "engine.h"
// int main(const int argc, char* argv[]) {
//     if (argc < 2) {
//         std::cerr << "Usage: " << argv[0] << " <topic>" << '\n';
//         return 1;
//     }
int main() {

    const std::string brokers = "localhost:9092";
    const std::string topic = "BTC_USDT";
    const std::string redisAddress = "redis://localhost:6379";
    const std::string pair = "BTC/USDT";

    Engine engine(brokers, topic, redisAddress, pair);
    engine.start();

}
