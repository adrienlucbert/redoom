#include <redoom/events/EventQueue.hh>

namespace redoom::events
{
void EventQueue::push(Event event) noexcept
{
  auto lock = std::unique_lock{*this->mutex};
  this->queue.push(event);
}

Event EventQueue::pop() noexcept
{
  auto lock = std::unique_lock{*this->mutex};
  auto event = this->queue.front();
  this->queue.pop();
  return event;
}

bool EventQueue::empty() const noexcept
{
  auto lock = std::unique_lock{*this->mutex};
  return this->queue.empty();
}
} // namespace redoom::events
