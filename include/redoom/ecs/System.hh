#pragma once

#include <Utils/UniqueTypeId.hh>
#include <redoom/ecs/SystemBase.hh>

namespace redoom::ecs
{
namespace detail
{
template <typename T>
class System : public SystemBase, public Utils::UniqueTypeId<T>
{
public:
  explicit System(bool is_multithreaded) noexcept
    : SystemBase{is_multithreaded}
  {
  }
};
} // namespace detail

template <typename T>
class MultithreadedSystem : public detail::System<T>
{
protected:
  MultithreadedSystem() noexcept
    : detail::System<T>(true)
  {
  }
};

template <typename T>
class MonothreadedSystem : public detail::System<T>
{
protected:
  MonothreadedSystem() noexcept
    : detail::System<T>(false)
  {
  }
};
} // namespace redoom::ecs
