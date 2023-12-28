#pragma once

#include <gopp/async/detail/shared_state.hpp>

#include <cassert>

namespace gopp::async {

template <typename T>
class Future {
  template <typename U>
  friend class Promise;

 public:
  Future(const Future&) = delete;
  Future& operator=(const Future&) = delete;

  Future(Future&&) = default;
  Future& operator=(Future&&) = default;

  // One-shot
  // Wait for result (value or exception)
  T Get() {
    return shared_state_->GetValue();
  }

 private:
  Future(
    std::shared_ptr<detail::SharedState<T>> shared_state
  ): shared_state_(shared_state){}

 private:
  std::shared_ptr<detail::SharedState<T>> shared_state_;
};

}  // namespace gopp::async
