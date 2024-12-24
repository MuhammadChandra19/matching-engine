//
// Created by Muhammad chandra zulfikar on 20/12/24.
//
 #include "engine.h"

Engine::Engine(const std::string &brokers, const std::string &topic, const std::string &redisAddress, const std::string &pair)
    :
        orderOffset(0),
        consumer(&matchingService, brokers, topic, 0),
        redisSnapshotStore(pair, redisAddress)
{
    const auto snapshot = redisSnapshotStore.loadStore();
    restore(snapshot);

    std::cout << "Engine Initialization." << '\n';
};

Engine::~Engine() {
    stopFlag.store(true);
    if (consumerThread.joinable()) {
        consumerThread.join();
    }
    if (snapshotThread.joinable()) {
        snapshotThread.join();
    }
}

void Engine::start()
{
    consumerThread = std::thread(&Engine::consumerThread, this);
    snapshotThread = std::thread(&Engine::snapshotThread, this);
}

void Engine::restore(const Snapshot& snapshot)
{
    orderOffset = snapshot.orderOffset;
    matchingService.orderBook->Restore(snapshot.orderBookSnapshot);
    std::cout << "Engine Restore." << '\n';
}

void Engine::runConsumer()
{
    consumer.start();
}


void Engine::runSnapshotStore()
{
    while (!stopFlag.load())
    {
        std::this_thread::sleep_for(std::chrono::seconds(30));
        const auto orderBookSnapshot = matchingService.orderBook->CreateSnapshot();
        Snapshot const snapshot(orderBookSnapshot, orderOffset.load());
        redisSnapshotStore.store(snapshot);
    }
}

