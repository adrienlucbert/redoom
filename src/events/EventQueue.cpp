#include <redoom/events/EventQueue.hh>

namespace redoom::events
{
void EventQueue::push(Event event) noexcept
{
  auto lock = std::unique_lock{*this->mutex_};
  this->queue_.push(event);
}

Event EventQueue::pop() noexcept
{
  auto lock = std::unique_lock{*this->mutex_};
  auto event = this->queue_.front();
  this->queue_.pop();
  return event;
}

bool EventQueue::empty() const noexcept
{
  auto lock = std::unique_lock{*this->mutex_};
  return this->queue_.empty();
}
} // namespace redoom::events
