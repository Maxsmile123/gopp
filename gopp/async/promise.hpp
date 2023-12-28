#pragma once

#include <gopp/async/future_impl.hpp>

#include <memory>


namespace gopp::async {
template <typename T>
class Promise {
 public:
  Promise() {
    this->shared_state_ = std::make_shared<detail::SharedState<T>>();
  };

  Promise(const Promise&) = delete;
  Promise& operator=(const Promise&) = delete;

  Promise(Promise&&) = default;
  Promise& operator=(Promise&&) = default;

  // One-shot
  Future<T> MakeFuture() {
    return Future<T>(this->shared_state_);
  }

  // One-shot
  void SetValue(T value) {
    shared_state_->SetValue(std::move(value));
  }

  // One-shot
  void SetException(std::exception_ptr eptr) {
    shared_state_->SetExc(eptr);
  }
  
 private:
  std::shared_ptr<detail::SharedState<T>> shared_state_;
};

}  // namespace stdlike
