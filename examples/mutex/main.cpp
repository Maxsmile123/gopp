#include <gopp/thread/mutex/mutex.hpp>

#include <thread>
#include <cassert>

int main() {
    const int kIterations = 128000;

    int a = 0;
    gopp::thread::stdlike::Mutex mutex;

    std::thread t1([&](){
        mutex.lock();
        for (int i = 0; i < kIterations; ++i) {
            ++a;
        }
        mutex.unlock();
    });
    std::thread t2([&](){
        std::lock_guard lock(mutex);
        for (int i = 0; i < kIterations; ++i) {
            ++a;
        }
    });
    t1.join();
    t2.join();
    assert(a == 2 * kIterations);
}