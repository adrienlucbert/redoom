#pragma once

namespace redoom::ecs
{
class ComponentBase
{
public:
  ComponentBase(ComponentBase const& b) noexcept = delete;
  ComponentBase(ComponentBase&& b) noexcept = delete;
  virtual ~ComponentBase() noexcept = default;

  ComponentBase& operator=(ComponentBase const& rhs) noexcept = delete;
  ComponentBase& operator=(ComponentBase&& rhs) noexcept = delete;

protected:
  ComponentBase() noexcept = default;

  [[nodiscard]] static unsigned int getNextTypeId() noexcept;
};
} // namespace redoom::ecs
