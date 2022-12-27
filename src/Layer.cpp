#include <redoom/Layer.hh>

namespace redoom
{
void Layer::onAttach() noexcept
{
}

void Layer::onDetach() noexcept
{
}

void Layer::onUpdate(double /*elapsed_time*/) noexcept
{
}

void Layer::afterUpdate() noexcept
{
}

EventPropagation Layer::onEvent(events::Event const& /*event*/) noexcept
{
  return Forward;
}
} // namespace redoom
