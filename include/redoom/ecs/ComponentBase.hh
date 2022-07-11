#pragma once

#include <string>

#include <redoom/ecs/Entity.hh>

namespace redoom::ecs
{
class Context;

class ComponentBase
{
public:
  ComponentBase(ComponentBase const&) noexcept = default;
  ComponentBase(ComponentBase&&) noexcept = default;
  virtual ~ComponentBase() noexcept = default;

  ComponentBase& operator=(ComponentBase const&) noexcept = default;
  ComponentBase& operator=(ComponentBase&&) noexcept = default;

  [[nodiscard]] virtual std::string const& getType() const noexcept = 0;

protected:
  ComponentBase() noexcept = default;
};
} // namespace redoom::ecs
