//
// Created by Muhammad chandra zulfikar on 20/12/24.
//
#include "engine.h"
#include <csignal>
bool Engine::stopFlag_ = false;
Engine::Engine(const std::string &brokers, const std::string &topic, const std::string &redisAddress, const std::string &pair)
    :
        orderOffset(0),
        consumer(&matchingService, brokers, topic, 0, stopFlag_),
        redisSnapshotStore(pair, redisAddress)
{
    const auto snapshot = redisSnapshotStore.loadStore();
    restore(snapshot);

    // Setup signal handling for graceful shutdown
    std::signal(SIGINT, stop);
    std::cout << "Engine Initialization." << '\n';
};

Engine::~Engine() {
    if (consumerThread.joinable()) {
        consumerThread.join();
    }
    if (snapshotThread.joinable()) {
        snapshotThread.join();
    }
}

void Engine::start()
{
    consumerThread = std::thread(&Engine::runConsumer, this);
    snapshotThread = std::thread(&Engine::runSnapshotStore, this);
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
    std::cout << "Snapshot executed Restore initiated" << '\n';
    while (!stopFlag_)
    {
        std::cout << "Snapshot executed Restore." << '\n';
        std::this_thread::sleep_for(std::chrono::seconds(30));
        const auto orderBookSnapshot = matchingService.orderBook->CreateSnapshot();

        auto snapshot =  Snapshot {
        .orderBookSnapshot=orderBookSnapshot,
        .orderOffset = orderOffset,};
        redisSnapshotStore.store(snapshot);
    }
}

void Engine::stop(const int sig)
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

