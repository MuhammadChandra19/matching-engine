//
// Created by Muhammad chandra zulfikar on 20/12/24.
//
#include "engine.h"
#include <csignal>
bool Engine::stopFlag_ = false;
Engine::Engine(const std::string &brokers,
               const std::string &topic,
               const std::string &redisAddress,
               const std::string &pair) :
    orderOffset(0),
    orderReader(brokers, topic, orderOffset, stopFlag_),
    redisSnapshotStore(pair, redisAddress)
{
    const auto snapshot = redisSnapshotStore.loadStore();
    restore(snapshot);

    // Setup signal handling for graceful shutdown
    std::signal(SIGINT, stop);
    std::cout << "Engine Initialization." << '\n';
};

Engine::~Engine() {
    if (orderReaderThread.joinable())
    {
        orderReaderThread.join();
    }
    if (orderApplierThread.joinable())
    {
        orderApplierThread.join();
    }
    if (snapshotThread.joinable()) {
        snapshotThread.join();
    }
}

void Engine::start()
{
    // consumerThread = std::thread(&Engine::runConsumer, this);
    orderReaderThread = std::thread(&Engine::runOrderReader, this);
    orderApplierThread = std::thread(&Engine::runOrderApplier, this);
    snapshotThread = std::thread(&Engine::runSnapshotStore, this);
}

void Engine::restore(const Snapshot& snapshot)
{
    orderOffset = snapshot.orderOffset;
    matchingService->orderBook->Restore(snapshot.orderBookSnapshot);
    std::cout << "Engine Restore." << '\n';
}

void Engine::runSnapshotStore()
{
    std::cout << "Snapshot executed Restore initiated" << '\n';
    while (!stopFlag_)
    {
        std::cout << "Snapshot executed Restore." << '\n';
        std::this_thread::sleep_for(std::chrono::seconds(30));
        const auto orderBookSnapshot = matchingService->orderBook->CreateSnapshot();

        auto snapshot =  Snapshot {
        .orderBookSnapshot=orderBookSnapshot,
        .orderOffset = orderOffset,};
        redisSnapshotStore.store(snapshot);
    }
}

void Engine::runOrderReader()
{
    std::cout << "Order Reader Initiated" << '\n';
    int offset = orderOffset;
    if (offset < 0)
    {
        offset++;
    }
    orderReader.setOffset(offset);

    while (!stopFlag_)
    {
        try
        {
            std::cout << "Order Reader." << '\n';
            auto placedOrder = orderReader.fetchOrder();
            orderChan.send(&placedOrder);
        }catch (std::exception& e)
        {
            // Adding logs
            std::cout << e.what() << '\n';

        }

    }
}

void Engine::runOrderApplier()
{
    std::cout << "Order Committer Initiated" << '\n';
    while (!stopFlag_)
    {
        auto value = orderChan.receive();
        if (!value.has_value()) {
            break; // Channel is closed
        }

        if (const auto *const placedOrder = value.value(); placedOrder->orderType == 0)
        {
            matchingService->handleCancelOrder(*placedOrder);
        } else
        {
            auto matchOrder = matchingService->handlePlaceOrder(*placedOrder);
        }
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
