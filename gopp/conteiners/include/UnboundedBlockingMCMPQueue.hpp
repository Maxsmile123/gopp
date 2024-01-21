#pragma once

#include <gopp/thread/include/mutex.hpp>
#include <gopp/thread/include/condvar.hpp>

#include <atomic>
#include <deque>
#include <optional>
#include <iostream>
#include <vector>


namespace gopp:conteiners {
// Unbounded Blocking Multi-Producer/Multi-Consumer (MPMC) Queue

template <typename T>
class UnboundedBlockingQueue {
 public:

  bool Put(T value) {
    std::lock_guard<gopp::thread::sync::Mutex> lock(mutex_);
    if (is_closed_.load()) {
      return false;
    }

    was_empty_.store(buffer_.empty());

    buffer_.push_back(std::move(value));
    if (was_empty_.load()) {
      no_empty_.notify_one();
    }
    return true;
  }

  std::optional<T> Take() {
    std::unique_lock<gopp::thread::sync::Mutex> lock(mutex_);

    while (buffer_.empty() && !is_closed_.load()) {
      no_empty_.wait(lock);
    }

    if (is_closed_.load() && buffer_.empty()) {
      return std::nullopt;
    }

    auto value = std::move(buffer_.front());
    buffer_.pop_front();
    return std::move(value);
  }

  void Close() {
    std::unique_lock<gopp::thread::sync::Mutex> lock(mutex_);
    is_closed_.store(true);
    no_empty_.notify_all();
  }

 private:
  std::deque<T> buffer_;
  gopp::thread::sync::CondVar no_empty_;
  gopp::thread::sync::Mutex mutex_;
  std::atomic<bool> is_closed_{false};
  std::atomic<bool> was_empty_{true};
};

} // namespace gopp:conteiners
