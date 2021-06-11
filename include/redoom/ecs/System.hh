#pragma once

namespace redoom::ecs
{
class System
{
public:
  System(System const& b) noexcept = delete;
  System(System&& b) noexcept = default;
  virtual ~System() noexcept;

  System& operator=(System const& rhs) noexcept = delete;
  System& operator=(System&& rhs) noexcept = delete;

  virtual void update(long elapsed_time) noexcept = 0;

protected:
  System() noexcept = default;
};
} // namespace redoom::ecs
