//
// Created by Muhammad chandra zulfikar on 20/12/24.
//
 #include "engine.h"

Engine::Engine(const RedisSnapshotStore redisSnapshotStore) :
    consumer(matchingService.get(), "localhost:19092", "eth-btc"),
    redisSnapshotStore(redisSnapshotStore)
{
    std::cout << "Engine Initialization." << '\n';
};