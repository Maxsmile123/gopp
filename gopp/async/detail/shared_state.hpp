#pragma once

#include <atomic>
#include <memory>
#include <variant>

namespace gopp::async::detail {

template <typename T>
class SharedState {
 public:
    SharedState(){
    this->value_ = std::make_shared<std::variant<T, std::exception_ptr>>(std::exception_ptr(nullptr));
    this->is_set_ = std::make_shared<std::atomic<bool>>(false);
 }
    // Non-copyable
  SharedState(const SharedState&) = delete;
  SharedState& operator=(const SharedState&) = delete;

  // Movable
  SharedState(SharedState&&) = default;
  SharedState& operator=(SharedState&&) = default;

  // One-shot
  T GetValue() {
    while(!(is_set_->load())){};
    try{
      std::rethrow_exception(std::get<std::exception_ptr>(*value_));
    } catch(std::bad_variant_access&){
      return std::move(std::get<T>(*value_));
    }
  }

  void SetValue(T value){
    *(this->value_) = std::move(value);
    is_set_->store(true);
  }

  void SetExc(std::exception_ptr value){
    *(this->value_) = value;
    is_set_->store(true);
  }

 private:
  std::shared_ptr<std::variant<T, std::exception_ptr>> value_;
  std::shared_ptr<std::atomic<bool>> is_set_;
};

template <>
class SharedState<std::exception_ptr>{
 public:

 SharedState(){
    this->value_ = std::make_shared<std::exception_ptr>(nullptr);
    is_set_value_ = std::make_shared<std::atomic<bool>>(false);
    is_set_exc_ = std::make_shared<std::atomic<bool>>(false);
 }

  SharedState(const SharedState&) = delete;
  SharedState& operator=(const SharedState&) = delete;


  SharedState(SharedState&&) = default;
  SharedState& operator=(SharedState&&) = default;


  std::exception_ptr GetValue() {
    while(!(is_set_value_->load() || is_set_exc_->load())){};
    if (is_set_value_->load()){
      return *value_;
    } else{
      std::rethrow_exception(*value_);
    }
  }

  void SetValue(std::exception_ptr value){
    *(this->value_) = std::move(value);
    is_set_value_->store(true);
  }

  void SetExc(std::exception_ptr value){
    *(this->value_) = std::move(value);
    is_set_exc_->store(true);
  }

 private:
  std::shared_ptr<std::exception_ptr> value_;
  std::shared_ptr<std::atomic<bool>> is_set_value_;
  std::shared_ptr<std::atomic<bool>> is_set_exc_;
};

}