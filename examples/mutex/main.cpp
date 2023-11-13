#include <gopp/thread/mutex/mutex.hpp>

#include <thread>
#include <cassert>


int main() {
    const int kIterations = 128;

    int a = 0;
    gopp::thread::std::Mutex mutex;

    std::thread t1([&](){
        mutex.lock();
        for (int i = 0; i < kIterations; ++i) {
            ++a;
        }
        mutex.unlock();
    });
    std::thread t2([&](){
        mutex.lock();
        for (int i = 0; i < kIterations; ++i) {
            ++a;
        }
        mutex.unlock();
    });

    assert(a == 2 * kIterations);
}