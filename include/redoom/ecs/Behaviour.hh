#pragma once

#include <redoom/ecs/Entity.hh>
#include <redoom/ecs/UpdateContext.hh>

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
};
} // namespace redoom::ecs
