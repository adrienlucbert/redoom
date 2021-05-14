#pragma once

namespace redoom::ecs
{
class System
{
public:
  virtual void update(long elapsed_time) noexcept = 0;

protected:
  System() noexcept = default;
  System(System const& b) noexcept = default;
  System(System&& b) noexcept = default;
  virtual ~System() noexcept = default;

  System& operator=(System const& rhs) noexcept = default;
  System& operator=(System&& rhs) noexcept = default;
};
} // namespace redoom::ecs
