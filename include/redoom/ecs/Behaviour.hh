#pragma once

#include <redoom/ecs/Entity.hh>
#include <redoom/ecs/UpdateContext.hh>
#include <redoom/events/Event.hh>

namespace redoom::ecs
{
class Behaviour
{
public:
  Behaviour() noexcept = default;
  Behaviour(Behaviour const& b) noexcept = delete;
  Behaviour(Behaviour&& b) noexcept = default;
  virtual ~Behaviour() noexcept = default;

  Behaviour& operator=(Behaviour const& rhs) noexcept = delete;
  Behaviour& operator=(Behaviour&& rhs) noexcept = default;

  virtual void onUpdate(Entity entity, UpdateContext& context) noexcept;

  virtual void onWindowResize(
      Entity entity, events::WindowResizeEvent& event) noexcept;
  virtual void onWindowClose(
      Entity entity, events::WindowCloseEvent& event) noexcept;

  virtual void onKey(Entity entity, events::KeyEvent& event) noexcept;
  virtual void onChar(Entity entity, events::CharEvent& event) noexcept;

  virtual void onMouseButton(
      Entity entity, events::MouseButtonEvent& event) noexcept;
  virtual void onMouseMove(
      Entity entity, events::MouseMoveEvent& event) noexcept;
  virtual void onScroll(Entity entity, events::ScrollEvent& event) noexcept;
};
} // namespace redoom::ecs