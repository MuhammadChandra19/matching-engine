//
// Created by Muhammad chandra zulfikar on 21/12/24.
//

#ifndef SNAPSHOT_H
#define SNAPSHOT_H

#include <order_book.h>
#include <nlohmann/json.hpp>

struct Snapshot {
      OrderBookSnapshot orderBookSnapshot;
      int64_t orderOffset;
      NLOHMANN_DEFINE_TYPE_INTRUSIVE(Snapshot, orderBookSnapshot, orderOffset)
};

#endif //SNAPSHOT_H
