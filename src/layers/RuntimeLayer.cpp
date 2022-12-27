#include <redoom/layers/RuntimeLayer.hh>

#include <redoom/Runtime.hh>

namespace redoom
{
void RuntimeLayer::onUpdate(double elapsed_time) noexcept
{
  Runtime::get().onUpdate(elapsed_time);
}

void RuntimeLayer::afterUpdate() noexcept
{
  Runtime::get().afterUpdate();
}

EventPropagation RuntimeLayer::onEvent(events::Event const& event) noexcept
{
  Runtime::get().onEvent(event);
  return Forward;
}
} // namespace redoom
