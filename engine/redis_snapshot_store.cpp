//
// Created by Muhammad chandra zulfikar on 21/12/24.
//

#include "redis_snapshot_store.h"
#include <iostream>
#include <nlohmann/json.hpp>
RedisSnapshotStore::RedisSnapshotStore(const std::string pair, const std::string &address)
    : pair(pair), redisClient(address)
{

    std::cout << "RedisSnapshotStore Initialization." << '\n';
}

void RedisSnapshotStore::store(const Snapshot& snapshot)
{
    try
    {
        const std::string jsonString = nlohmann::json(snapshot).dump();
        constexpr std::chrono::milliseconds ttl(7 * 24 * 60 * 60 * 1000);
        redisClient.set(pair, jsonString,  ttl);
    } catch (const sw::redis::Error &e)
    {
        std::cerr << e.what() << '\n';
    }
}

Snapshot RedisSnapshotStore::loadStore()
{
    try
    {
        if (auto snapshot = redisClient.get(pair))
        {
            auto deserializedSnapshot = nlohmann::json::parse(*snapshot).get<Snapshot>();
        }


        return Snapshot{};
    }
    catch (const sw::redis::Error &e)
    {
        std::cerr << e.what() << '\n';
    }
    return {};
}

