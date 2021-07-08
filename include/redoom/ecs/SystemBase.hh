#pragma once

#include <redoom/ecs/ComponentManager.hh>

namespace redoom::ecs
{
class SystemBase
{
public:
  SystemBase(SystemBase const& b) noexcept = default;
  SystemBase(SystemBase&& b) noexcept = default;
  virtual ~SystemBase() noexcept = default;

  SystemBase& operator=(SystemBase const& rhs) noexcept = default;
  SystemBase& operator=(SystemBase&& rhs) noexcept = default;

  virtual void update(long /*unused*/) noexcept;
  virtual void update(long /*unused*/, ComponentManager& /*unused*/) noexcept;

protected:
  SystemBase() noexcept = default;
};
} // namespace redoom::ecs
