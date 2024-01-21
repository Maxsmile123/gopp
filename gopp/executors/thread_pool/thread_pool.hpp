#pragma once

#include <gopp/conteinters/include/UnboundedBlockingMCMPQueue.hpp>
#include <gopp/executors/detail/task.hpp>

#include <gopp/thread/include/wait_group.hpp>
#include <gopp/thread/include/mutex.hpp>

#include <vector>
#include <iostream>
#include <cassert>

namespace gopp::executors {

// Fixed-size pool of worker threads

class ThreadPool {
 public:
  explicit ThreadPool(size_t threads);
  ~ThreadPool();

  // Non-copyable
  ThreadPool(const ThreadPool&) = delete;
  ThreadPool& operator=(const ThreadPool&) = delete;

  // Non-movable
  ThreadPool(ThreadPool&&) = delete;
  ThreadPool& operator=(ThreadPool&&) = delete;

  // Launches worker threads
  void Start();

  // Schedules task for execution in one of the worker threads
  void Submit(Task);

  // Locates current thread pool from worker thread
  static ThreadPool* Current();

  // Waits until outstanding work count reaches zero
  void WaitIdle();

  // Stops the worker threads as soon as possible
  void Stop();

 private:
  void Do();


 private:
  const size_t threads_number_;
  gopp::thread::sync::Mutex mutex_;
  std::vector<std::thread> workers_;
  gopp::conteiners::UnboundedBlockingQueue<gopp::executors::detail::Task> task_queue_;
  gopp::thread::sync::WaitGroup task_progress;
  std::atomic<int32_t> is_stoped{0};

};

}  // namespace tp
