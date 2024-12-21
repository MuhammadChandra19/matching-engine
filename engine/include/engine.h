//
// Created by Muhammad chandra zulfikar on 20/12/24.
//

#ifndef ENGINE_H
#define ENGINE_H

#include "trade_consumer.h"
#include "redis_snapshot_store.h"
class Engine {
    public:
        explicit Engine(RedisSnapshotStore redisSnapshotStore);

        void start();
        ~Engine();

    private:
        /**
            The matchingService held by the engine, corresponding to the product.
            It requires snapshots and recovery from snapshots.
        */
        std::shared_ptr<const MatchingService> matchingService;
        TradeConsumer consumer;
        RedisSnapshotStore redisSnapshotStore;

        void runConsumer();
        void runSnapshotStore();
};

#endif //ENGINE_H
