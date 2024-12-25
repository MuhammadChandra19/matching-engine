//
// Created by Muhammad chandra zulfikar on 20/12/24.
//

#ifndef ENGINE_H
#define ENGINE_H
#include <matching_service.h>
#include "redis_snapshot_store.h"
#include "trade_consumer.h"

#include <queue>
class Engine {
    public:
        explicit Engine(const std::string &brokers, const std::string &topic, const std::string &redisAddress, const std::string &pair);

        void start();
        ~Engine();

    private:
        static bool stopFlag_;
        std::atomic<int64_t> orderOffset;
        /**
            The matchingService held by the engine, corresponding to the product.
            It requires snapshots and recovery from snapshots.
        */
        std::unique_ptr<MatchingService> matchingService;
        TradeConsumer consumer;
        RedisSnapshotStore redisSnapshotStore;


        std::mutex queueMutex;
        std::condition_variable queueCond;
        std::queue<std::string> orderQueue;  // Queue to hold raw order messages

        std::thread consumerThread;
        std::thread snapshotThread;

        void runConsumer();
        static void stop(int sig);
        void runSnapshotStore();
        void restore(const Snapshot& snapshot);
};

#endif //ENGINE_H
