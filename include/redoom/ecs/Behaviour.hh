#pragma once

#include <redoom/ecs/Context.hh>
#include <redoom/ecs/Entity.hh>
#include <redoom/ecs/UpdateContext.hh>
#include <redoom/events/Event.hh>

namespace redoom::ecs
{
class Behaviour
{
public:
  Behaviour() noexcept = default;
  Behaviour(Behaviour const&) noexcept = delete;
  Behaviour(Behaviour&&) noexcept = default;
  virtual ~Behaviour() noexcept = default;

  Behaviour& operator=(Behaviour const&) noexcept = delete;
  Behaviour& operator=(Behaviour&&) noexcept = default;

  [[nodiscard]] virtual std::string const& getType() const noexcept = 0;

  virtual void onInit(Entity entity, Context& context) noexcept;
  virtual void onDestroy(Entity entity, Context& context) noexcept;

  virtual void onUpdate(Entity entity, UpdateContext& context) noexcept;

  virtual void onWindowFocus(
      Entity entity, events::WindowFocusEvent& event) noexcept;
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
