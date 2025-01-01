#include "Log.h"
#include <ctime>

// Base structure constructor
Base::Base(LogType logType, int64_t seq, const std::string& pair, std::time_t logTime)
    : type(logType), sequence(seq), pair(pair), time(logTime) {}

// ReceivedLog implementation
ReceivedLog::ReceivedLog(int64_t seq, const std::string& pair, int64_t order, const double& orderSize,
                         const double& orderPrice, const int64_t& type)
    : orderId(order), size(orderSize), price(orderPrice),
      orderType(type), base(LogType::Open, seq, pair, std::time(nullptr)) {}

int64_t ReceivedLog::GetSeq() const {
    return base.sequence;
}

// OpenLog implementation
OpenLog::OpenLog(int64_t seq, const std::string& pair, int64_t order, const double& remaining,
                 const double& orderPrice, bool bid)
    : orderId(order), size(remaining), price(orderPrice), bid(bid),
      base(LogType::Open, seq, pair, std::time(nullptr)) {}

int64_t OpenLog::GetSeq() const {
    return base.sequence;
}

// DoneLog implementation
DoneLog::DoneLog(int64_t seq, const std::string& pair, int64_t order, const double& remaining,
                 const double& orderPrice, const std::string& doneReason, const bool bid)
    : orderId(order), price(orderPrice), remainingSize(remaining),
      reason(doneReason), bid(bid), base(LogType::Done, seq, pair, std::time(nullptr)) {}

int64_t DoneLog::GetSeq() const {
    return base.sequence;
}

// MatchLog implementation
MatchLog::MatchLog(int64_t seq, const std::string& pair, int64_t takerOrderId,
                   int64_t makerOrderId, const double& matchPrice, const double& matchSize)
    : takerOrderId(takerOrderId), makerOrderId(makerOrderId),
      price(matchPrice), size(matchSize), base(LogType::Match, seq, pair, std::time(nullptr)) {}

int64_t MatchLog::GetSeq() const {
    return base.sequence;
}
