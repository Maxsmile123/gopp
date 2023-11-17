#include <gopp/thread/include/mutex.hpp>
#include <gopp/thread/include/condvar.hpp>

#include <thread>
#include <cassert>
#include <iostream>
#include <chrono>

using namespace std::chrono_literals;

class Event {
   public:
    void Await() {
      std::unique_lock lock(mutex_);
      while (!set_) {
        set_cond_.Wait(lock);
      }
    }

    void Set() {
      std::lock_guard guard(mutex_);
      set_ = true;
      set_cond_.NotifyOne();
    }

    void Reset() {
      std::lock_guard guard(mutex_);
      set_ = false;
    }

   private:
    bool set_{false};
    gopp::thread::sync::Mutex mutex_;
    gopp::thread::sync::CondVar set_cond_;
  };

int main() {
    Event pass;

    for (size_t i = 0; i < 3; ++i) {
      pass.Reset();

      bool passed = false;

      std::thread waiter([&]() {
          {
            auto start = std::chrono::steady_clock::now();
            pass.Await();
            auto end = std::chrono::steady_clock::now();
            auto elapsed = duration_cast<std::chrono::milliseconds>(end - start);
            assert(elapsed < 1001ms);
          }
          passed = true;
      });

      std::this_thread::sleep_for(1s);

      assert(passed == false);

      pass.Set();
      waiter.join();

      assert(passed);
    }
}