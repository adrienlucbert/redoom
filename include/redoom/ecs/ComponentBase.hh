#pragma once

#include <redoom/ecs/Entity.hh>

namespace redoom::ecs
{
struct Context;

class ComponentBase
{
public:
  ComponentBase(ComponentBase const& b) noexcept = default;
  ComponentBase(ComponentBase&& b) noexcept = default;
  virtual ~ComponentBase() noexcept = default;

  ComponentBase& operator=(ComponentBase const& rhs) noexcept = default;
  ComponentBase& operator=(ComponentBase&& rhs) noexcept = default;

  virtual void onInit(Entity entity, Context& /*context*/) noexcept;
  virtual void onDestroy(Entity entity, Context& /*context*/) noexcept;

protected:
  ComponentBase() noexcept = default;
};
} // namespace redoom::ecs
