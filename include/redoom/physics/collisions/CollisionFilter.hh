#pragma once

namespace redoom::physics
{
class CollisionFilter
{
public:
  CollisionFilter() noexcept = default;
  CollisionFilter(CollisionFilter const& b) noexcept = default;
  CollisionFilter(CollisionFilter&& b) noexcept = default;
  virtual ~CollisionFilter() noexcept = default;

  CollisionFilter& operator=(CollisionFilter const& rhs) noexcept = default;
  CollisionFilter& operator=(CollisionFilter&& rhs) noexcept = default;

private:
};
} // namespace redoom::physics
