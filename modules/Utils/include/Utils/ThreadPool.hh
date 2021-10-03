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
  ThreadPool(ThreadPool const& b) noexcept = delete;
  ThreadPool(ThreadPool&& b) noexcept = default;
  ~ThreadPool() noexcept
  {
    {
      auto lock = std::unique_lock{*this->mutex};
      this->is_stopping = true;
    }
    this->cv->notify_all();
    for (auto& worker : this->workers)
      worker.join();
  }

  ThreadPool& operator=(ThreadPool const& rhs) noexcept = delete;
  ThreadPool& operator=(ThreadPool&& rhs) noexcept = default;

  template <typename Callable, typename... Args>
  std::future<typename std::result_of_t<Callable(Args...)>> enqueue(
      Callable&& f, Args&&... args)
  {
    using return_type = typename std::result_of_t<Callable(Args...)>;

    if (this->workers.empty())
      assert("Cannot enqueue task on empty ThreadPool" == nullptr);
    auto task = std::packaged_task<return_type()>{
        [&f, &args...]() { f(std::forward<Args>(args)...); }};
    auto res = task.get_future();
    {
      auto lock = std::unique_lock{*this->mutex};
      if (this->is_stopping)
        assert("Cannot enqueue task on stopped ThreadPool" == nullptr);
      this->tasks.emplace(std::move(task));
    }
    this->cv->notify_one();
    return res;
  }

  void reserve(unsigned long desired_size) noexcept
  {
    while (desired_size > this->workers.size())
      this->spawnWorker();
  }

  [[nodiscard]] auto size() const noexcept
  {
    return this->workers.size();
  }

private:
  void spawnWorker() noexcept
  {
    auto queue_lock = std::unique_lock{*this->mutex};
    this->workers.emplace_back([this] {
      while (true) {
        Task task;
        {
          auto lock = std::unique_lock{*this->mutex};
          this->cv->wait(lock,
              [this] { return this->is_stopping || !this->tasks.empty(); });
          if (this->is_stopping && this->tasks.empty())
            return;
          task = std::move(this->tasks.front());
          this->tasks.pop();
        }
        task();
      }
    });
  }

  std::vector<std::thread> workers;
  std::queue<Task> tasks;
  std::unique_ptr<std::mutex> mutex{std::make_unique<std::mutex>()};
  std::unique_ptr<std::condition_variable> cv{
      std::make_unique<std::condition_variable>()};
  bool is_stopping{false};
};
} // namespace redoom::Utils
