#pragma once

#include <redoom/ecs/ComponentBase.hh>

namespace redoom::ecs
{
template <typename T>
class Component : public ComponentBase
{
public:
  Component(Component const& b) noexcept = default;
  Component(Component&& b) noexcept = default;
  ~Component() noexcept override = default;

  Component& operator=(Component const& rhs) noexcept = default;
  Component& operator=(Component&& rhs) noexcept = default;

  [[nodiscard]] static unsigned int getTypeId() noexcept
  {
    static unsigned int id = ComponentBase::getNextTypeId();
    return id;
  }

protected:
  Component() noexcept = default;
};
} // namespace redoom::ecs
