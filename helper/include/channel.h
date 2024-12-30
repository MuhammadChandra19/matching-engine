//
// Created by Muhammad chandra zulfikar on 29/12/24.
//
#include <iostream>
#include <thread>
#include <queue>
#include <mutex>
#include <condition_variable>
#include <optional>

#ifndef CHANNEL_H
#define CHANNEL_H
template<typename T>
class Channel {
private:
    std::queue<T> queue_;
    mutable std::mutex mutex_;
    std::condition_variable cond_var_;
    bool closed_ = false;
public:
    void send(const T& value);
    std::optional<T> receive();
    void close();
    bool is_closed() const;
};

#endif //CHANNEL_H
