//
// Created by Muhammad chandra zulfikar on 20/12/24.
//

#ifndef ENGINE_H
#define ENGINE_H
#include "redis_snapshot_store.h"
#include "trade_consumer.h"
#include <matching_service.h>

#include <channel.h>
#include <order_reader.h>
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
        OrderReader orderReader;
        RedisSnapshotStore redisSnapshotStore;

        std::thread snapshotThread;
        std::thread orderReaderThread;
        std::thread orderApplierThread;

        Channel<Snapshot *> snapshotChan;
        Channel<PlaceOrderRequest *> orderChan;

        void runConsumer();
        static void stop(int sig);
        void runSnapshotStore();
        void restore(const Snapshot& snapshot);
        void runOrderReader();
        void runOrderApplier();
};

#endif //ENGINE_H
