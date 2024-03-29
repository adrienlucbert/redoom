#pragma once

#include <memory>
#include <mutex>
#include <queue>

#include <redoom/events/Event.hh>

namespace redoom::events
{
class EventQueue
{
public:
  EventQueue() noexcept = default;
  EventQueue(EventQueue const&) noexcept = delete;
  EventQueue(EventQueue&&) noexcept = default;
  ~EventQueue() noexcept = default;

  EventQueue& operator=(EventQueue const&) noexcept = delete;
  EventQueue& operator=(EventQueue&&) noexcept = default;

  void push(Event event) noexcept;
  Event pop() noexcept;

  [[nodiscard]] bool empty() const noexcept;

private:
  mutable std::unique_ptr<std::mutex> mutex_{std::make_unique<std::mutex>()};
  std::queue<Event> queue_;
};
}; // namespace redoom::events
