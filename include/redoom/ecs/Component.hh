#pragma once

#include <Utils/UniqueTypeId.hh>
#include <redoom/ecs/ComponentBase.hh>
#include <redoom/ecs/Entity.hh>

namespace redoom::ecs
{
template <typename T>
class Component : public ComponentBase, public Utils::UniqueTypeId<T>
{
public:
  Component(Component const&) noexcept = default;
  Component(Component&&) noexcept = default;
  ~Component() noexcept override = default;

  Component& operator=(Component const&) noexcept = default;
  Component& operator=(Component&&) noexcept = default;

protected:
  Component() noexcept = default;
};
} // namespace redoom::ecs
