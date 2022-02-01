#pragma once

namespace redoom::physics
{
class World
{
public:
  World() noexcept = default;
  World(World const& b) noexcept = default;
  World(World&& b) noexcept = default;
  ~World() noexcept = default;

  World& operator=(World const& rhs) noexcept = default;
  World& operator=(World&& rhs) noexcept = default;

  void step(double timestep) noexcept;

private:
};
} // namespace redoom::physics
