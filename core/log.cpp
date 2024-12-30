#include <iostream>
#include <vector>
#include <string>
#include <ctime>
#include <iomanip>
#include <sstream>

// Enum for log types
enum class LogType {
    Match,
    Open,
    Done
};

// Base class for log entries
class Log {
public:
    virtual int64_t GetSeq() const = 0;  // Pure virtual function to be implemented by derived classes
    virtual ~Log() = default;
};

// Base structure for common fields
struct Base {
    LogType type;
    int64_t sequence;
    std::string pair;
    std::time_t time;

    Base(LogType logType, int64_t seq, const std::string& pair, std::time_t logTime)
        : type(logType), sequence(seq), pair(pair), time(logTime) {}
};

// Derived class for ReceivedLog
class ReceivedLog : public Log {
public:
    int64_t orderId;
    double size;
    double price;
    std::string side;
    std::string orderType;

    ReceivedLog(int64_t seq, const std::string& pair, int64_t order, const double& orderSize,
                const double& orderPrice, const std::string& orderSide, const std::string& type)
        : orderId(order), size(orderSize), price(orderPrice), side(orderSide),
          orderType(type), base(LogType::Open, seq, pair, std::time(nullptr)) {}

    int64_t GetSeq() const override {
        return base.sequence;
    }
private:
    Base base;
};

// Derived class for OpenLog
class OpenLog : public Log {
public:
    int64_t orderId;
    double remainingSize;
    double price;
    std::string side;

    OpenLog(int64_t seq, const std::string& pair, int64_t order, const double& remaining,
            const double& orderPrice, const std::string& orderSide)
        : orderId(order), remainingSize(remaining), price(orderPrice),
          side(orderSide), base(LogType::Open, seq, pair, std::time(nullptr)) {}

    int64_t GetSeq() const override {
        return base.sequence;
    }
private:
    Base base;
};

// Derived class for DoneLog
class DoneLog : public Log {
public:
    int64_t orderId;
    double price;
    double remainingSize;
    std::string reason;
    std::string side;

    DoneLog(int64_t seq, const std::string& pair, int64_t order, const double& remaining,
            const double& orderPrice, const std::string& doneReason, const std::string& orderSide)
        : orderId(order), price(orderPrice), remainingSize(remaining),
          reason(doneReason), side(orderSide), base(LogType::Done, seq, pair, std::time(nullptr)) {}

    int64_t GetSeq() const override {
        return base.sequence;
    }
private:
    Base base;
};

// Derived class for MatchLog
class MatchLog : public Log {
public:
    int64_t tradeId;
    int64_t takerOrderId;
    int64_t makerOrderId;
    std::string side;
    double price;
    double size;

    MatchLog(int64_t seq, const std::string& pair, int64_t tradeSeq, int64_t takerOrder,
             int64_t makerOrder, const double& matchPrice, const double& matchSize)
        : tradeId(tradeSeq), takerOrderId(takerOrder), makerOrderId(makerOrder),
          price(matchPrice), size(matchSize), base(LogType::Match, seq, pair, std::time(nullptr)) {}

    int64_t GetSeq() const override {
        return base.sequence;
    }
private:
    Base base;
};