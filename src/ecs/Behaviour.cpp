#include <redoom/ecs/Behaviour.hh>

namespace redoom::ecs
{
void Behaviour::onInit(Entity /*entity*/, Context& /*context*/) noexcept
{
}

void Behaviour::onDestroy(Entity /*entity*/, Context& /*context*/) noexcept
{
}

void Behaviour::onUpdate(Entity /*entity*/, UpdateContext& /*context*/) noexcept
{
}

void Behaviour::onWindowFocus(
    Entity /*entity*/, events::WindowFocusEvent& /*event*/) noexcept
{
}

void Behaviour::onWindowResize(
    Entity /*entity*/, events::WindowResizeEvent& /*event*/) noexcept
{
}

void Behaviour::onWindowClose(
    Entity /*entity*/, events::WindowCloseEvent& /*event*/) noexcept
{
}

void Behaviour::onKey(Entity /*entity*/, events::KeyEvent& /*event*/) noexcept
{
}

void Behaviour::onChar(Entity /*entity*/, events::CharEvent& /*event*/) noexcept
{
}

void Behaviour::onMouseButton(
    Entity /*entity*/, events::MouseButtonEvent& /*event*/) noexcept
{
}

void Behaviour::onMouseMove(
    Entity /*entity*/, events::MouseMoveEvent& /*event*/) noexcept
{
}

void Behaviour::onScroll(
    Entity /*entity*/, events::ScrollEvent& /*event*/) noexcept
{
}
} // namespace redoom::ecs
