#pragma once

#include <redoom/ecs/ComponentBase.hh>

namespace redoom::ecs
{
template <typename T>
class Component : public ComponentBase
{
public:
  Component(Component const& b) noexcept = delete;
  Component(Component&& b) noexcept = delete;
  ~Component() noexcept override = default;

  Component& operator=(Component const& rhs) noexcept = delete;
  Component& operator=(Component&& rhs) noexcept = delete;

  [[nodiscard]] static unsigned int getTypeId() noexcept
  {
    static unsigned int id = ComponentBase::getNextTypeId();
    return id;
  }

protected:
  Component() noexcept = default;
};
} // namespace redoom::ecs
