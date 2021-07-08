#pragma once

#include <Utils/UniqueTypeId.hh>
#include <redoom/ecs/SystemBase.hh>

namespace redoom::ecs
{
template <typename T>
class System : public SystemBase, public Utils::UniqueTypeId<T>
{
public:
  System(System const& b) noexcept = delete;
  System(System&& b) noexcept = default;
  ~System() noexcept override = default;

  System& operator=(System const& rhs) noexcept = delete;
  System& operator=(System&& rhs) noexcept = default;

protected:
  System() noexcept = default;
};
} // namespace redoom::ecs
