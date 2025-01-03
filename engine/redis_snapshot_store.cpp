//
// Created by Muhammad chandra zulfikar on 21/12/24.
//

#include "redis_snapshot_store.h"
#include <iostream>
#include <nlohmann/json.hpp>
#include <utility>
RedisSnapshotStore::RedisSnapshotStore(std::string  pair, const std::string &address)
    : pair(std::move(pair)), redisClient(address)
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
        Snapshot snapshot{};
        if (auto snapshotStr = redisClient.get(pair))
        {
            std::cout << snapshotStr.value() << '\n';
             snapshot = nlohmann::json::parse(*snapshotStr).get<Snapshot>();
        }


        return snapshot;
    }
    catch (const sw::redis::Error &e)
    {
        std::cerr << e.what() << '\n';
    }
    return {};
}

