//
// Created by Muhammad chandra zulfikar on 01/01/25.
//
#ifndef LOG_H
#define LOG_H

#include <string>
#include <ctime>

// Enum for log types
enum class LogType {
    Match,
    Open,
    Done
};

// Abstract base class for log entries
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

    Base(LogType logType, int64_t seq, const std::string& pair, std::time_t logTime);
};

// Derived class for ReceivedLog
class ReceivedLog : public Log {
public:
    int64_t orderId;
    double size;
    double price;
    int64_t orderType;

    ReceivedLog(int64_t seq, const std::string& pair, int64_t order, const double& orderSize,
                const double& orderPrice, const int64_t& type);

    int64_t GetSeq() const override;

private:
    Base base;
};

// Derived class for OpenLog
class OpenLog : public Log {
public:
    int64_t orderId;
    double size;
    double price;
    bool bid;

    OpenLog(int64_t seq, const std::string& pair, int64_t order, const double& remaining,
            const double& orderPrice, bool bid);

    int64_t GetSeq() const override;

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
    bool bid;

    DoneLog(int64_t seq, const std::string& pair, int64_t order, const double& remaining,
            const double& orderPrice, const std::string& doneReason, bool bid);

    int64_t GetSeq() const override;

private:
    Base base;
};

// Derived class for MatchLog
class MatchLog : public Log {
public:
    int64_t takerOrderId;
    int64_t makerOrderId;
    std::string side;
    double price;
    double size;

    MatchLog(int64_t seq, const std::string& pair, int64_t takerOrderId,
             int64_t makerOrderId, const double& matchPrice, const double& matchSize);

    int64_t GetSeq() const override;

private:
    Base base;
};

#endif // LOG_H
