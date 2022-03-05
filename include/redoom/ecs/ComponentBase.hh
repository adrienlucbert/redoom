#pragma once

#include <string>

#include <redoom/ecs/Entity.hh>

namespace redoom::ecs
{
class Context;

class ComponentBase
{
public:
  ComponentBase(ComponentBase const& b) noexcept = default;
  ComponentBase(ComponentBase&& b) noexcept = default;
  virtual ~ComponentBase() noexcept = default;

  ComponentBase& operator=(ComponentBase const& rhs) noexcept = default;
  ComponentBase& operator=(ComponentBase&& rhs) noexcept = default;

  [[nodiscard]] virtual std::string const& getType() const noexcept = 0;

protected:
  ComponentBase() noexcept = default;
};
} // namespace redoom::ecs
