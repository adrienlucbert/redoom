#pragma once

#include <Utils/UniqueTypeId.hh>
#include <redoom/ecs/ComponentBase.hh>

namespace redoom::ecs
{
template <typename T>
class Component : public ComponentBase, public Utils::UniqueTypeId<T>
{
public:
  Component(Component const& b) noexcept = default;
  Component(Component&& b) noexcept = default;
  ~Component() noexcept override = default;

  Component& operator=(Component const& rhs) noexcept = default;
  Component& operator=(Component&& rhs) noexcept = default;

protected:
  Component() noexcept = default;
};
} // namespace redoom::ecs
