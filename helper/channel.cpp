//
// Created by Muhammad chandra zulfikar on 29/12/24.
//
#include "channel.h"

/**
 * @brief Sends a value to the channel.
 *
 * This method pushes the given value into the channel's queue and notifies
 * any threads waiting to receive a value. If the channel is closed,
 * an exception is thrown.
 *
 * @param value The value to send into the channel.
 * @throws std::runtime_error If the channel is closed.
 */
template<typename T>
void Channel<T>::send(const T& value) {
    std::unique_lock lock(mutex_);
    if (closed_) {
        throw std::runtime_error("Cannot send to a closed channel");
    }
    queue_.push(value);
    cond_var_.notify_one();
}

/**
 * @brief Receives a value from the channel.
 *
 * This method waits until a value is available in the channel's queue or
 * the channel is closed. If a value is available, it is removed from the
 * queue and returned. If the channel is closed and the queue is empty,
 * it returns `std::nullopt`.
 *
 * @return std::optional<T> The received value, or `std::nullopt` if the
 * channel is closed and empty.
 */
template<typename T>
std::optional<T> Channel<T>::receive() {
    std::unique_lock lock(mutex_);
    cond_var_.wait(lock, [this]() { return !queue_.empty() || closed_; });
    if (queue_.empty() && closed_) {
        return std::nullopt; // Indicate the channel is closed
    }
    T value = queue_.front();
    queue_.pop();
    return value;
}

template<typename T>
void Channel<T>::close() {
    std::unique_lock lock(mutex_);
    closed_ = true;
    cond_var_.notify_all();
}

template<typename T>
bool Channel<T>::is_closed() const {
    std::unique_lock lock(mutex_);
    return closed_;
}
