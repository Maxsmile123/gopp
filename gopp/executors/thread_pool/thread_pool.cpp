#include <gopp/executors/thread_pool/thread_pool.hpp>

namespace gopp::executors {

thread_local ThreadPool* pool = nullptr

ThreadPool::ThreadPool(size_t threads): threads_number_(threads){}

void ThreadPool::Start() {
  for(size_t i = 0; i < threads_number_; ++i){
    workers_.emplace_back([this](){
      Do();
    });
  }
}

ThreadPool::~ThreadPool() {
  assert(is_stoped.load());
}

void ThreadPool::Submit(Task task) {
  std::lock_guard<gopp::thread::sync::Mutex> lock(mutex_);
  task_queue_.Put(std::move(task));
  task_progress.Add(1);
}

ThreadPool* ThreadPool::Current() {
  return pool;
}

void ThreadPool::WaitIdle() {
  task_progress.Wait();
}

void ThreadPool::Stop() {
  std::lock_guard<gopp::thread::sync::Mutex> lock(mutex_);
  task_queue_.Close();
  is_stoped.store(1);
  for (auto& thread: workers_){
    thread.join();
  }
}

void ThreadPool::Do(){
  pool = this;
  while(!is_stoped.load()){
    auto task = task_queue_.Take();
    if (task.has_value()){
      (*task)();
    }
    task_progress.Done();
  }

}

}  // namespace gopp::executors
