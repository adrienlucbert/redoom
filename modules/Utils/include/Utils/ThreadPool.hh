#pragma once

#include <cassert>
#include <condition_variable>
#include <functional>
#include <future>
#include <memory>
#include <mutex>
#include <queue>
#include <thread>
#include <vector>

namespace redoom::Utils
{
class ThreadPool
{
public:
  using Task = std::packaged_task<void()>;

  ThreadPool() noexcept = default;
  explicit ThreadPool(unsigned int size) noexcept
  {
    this->reserve(size);
  }
  ThreadPool(ThreadPool const&) noexcept = delete;
  ThreadPool(ThreadPool&&) noexcept = default;
  ~ThreadPool() noexcept
  {
    {
      auto lock = std::unique_lock{*this->mutex_};
      this->is_stopping_ = true;
    }
    this->cv_->notify_all();
    for (auto& worker : this->workers_)
      worker.join();
  }

  ThreadPool& operator=(ThreadPool const&) noexcept = delete;
  ThreadPool& operator=(ThreadPool&&) noexcept = default;

  template <typename Callable, typename... Args>
  std::future<typename std::result_of_t<Callable(Args...)>> enqueue(
      Callable&& f, Args&&... args)
  {
    using return_type = typename std::result_of_t<Callable(Args...)>;

    if (this->workers_.empty())
      assert("Cannot enqueue task on empty ThreadPool" == nullptr);
    auto task = std::packaged_task<return_type()>{
        [&f, &args...]() { f(std::forward<Args>(args)...); }};
    auto res = task.get_future();
    {
      auto lock = std::unique_lock{*this->mutex_};
      if (this->is_stopping_)
        assert("Cannot enqueue task on stopped ThreadPool" == nullptr);
      this->tasks_.emplace(std::move(task));
    }
    this->cv_->notify_one();
    return res;
  }

  void reserve(unsigned long desired_size) noexcept
  {
    while (desired_size > this->workers_.size())
      this->spawnWorker();
  }

  [[nodiscard]] auto size() const noexcept
  {
    return this->workers_.size();
  }

private:
  void spawnWorker() noexcept
  {
    auto queue_lock = std::unique_lock{*this->mutex_};
    this->workers_.emplace_back([this] {
      while (true) {
        Task task;
        {
          auto lock = std::unique_lock{*this->mutex_};
          this->cv_->wait(lock,
              [this] { return this->is_stopping_ || !this->tasks_.empty(); });
          if (this->is_stopping_ && this->tasks_.empty())
            return;
          task = std::move(this->tasks_.front());
          this->tasks_.pop();
        }
        task();
      }
    });
  }

  std::vector<std::thread> workers_;
  std::queue<Task> tasks_;
  std::unique_ptr<std::mutex> mutex_{std::make_unique<std::mutex>()};
  std::unique_ptr<std::condition_variable> cv_{
      std::make_unique<std::condition_variable>()};
  bool is_stopping_{false};
};
} // namespace redoom::Utils
