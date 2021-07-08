#pragma once

namespace redoom::ecs
{
class ComponentBase
{
public:
  ComponentBase(ComponentBase const& b) noexcept = default;
  ComponentBase(ComponentBase&& b) noexcept = default;
  virtual ~ComponentBase() noexcept = default;

  ComponentBase& operator=(ComponentBase const& rhs) noexcept = default;
  ComponentBase& operator=(ComponentBase&& rhs) noexcept = default;

protected:
  ComponentBase() noexcept = default;
};
} // namespace redoom::ecs
