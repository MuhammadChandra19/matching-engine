//
// Created by Muhammad chandra zulfikar on 20/12/24.
//

#ifndef REDIS_SNAPSHOT_STORE_H
#define REDIS_SNAPSHOT_STORE_H
#include <sw/redis++/redis++.h>
#include "snapshot.h"

class RedisSnapshotStore {
  public:
    std::string pair;
    RedisSnapshotStore(std::string pair, const std::string &address);
    void store(const Snapshot& snapshot);
    Snapshot loadStore();

  private:
    sw::redis::Redis redisClient;
};
#endif //REDIS_SNAPSHOT_STORE_H
