#pragma once

#include <functional>
#include <vector>

#include <redoom/physics/Body.hh>
#include <redoom/physics/collisions/CollisionManifold.hh>

namespace redoom::physics
{
class CollisionDetection
{
public:
  CollisionDetection() noexcept = default;
  CollisionDetection(CollisionDetection const& b) noexcept = default;
  CollisionDetection(CollisionDetection&& b) noexcept = default;
  ~CollisionDetection() noexcept = default;

  CollisionDetection& operator=(
      CollisionDetection const& rhs) noexcept = default;
  CollisionDetection& operator=(CollisionDetection&& rhs) noexcept = default;

  std::vector<CollisionManifold> getCollisions(
      std::vector<std::reference_wrapper<Body const>> bodies) noexcept;

private:
  std::vector<CollisionManifold> broadPhase(
      std::vector<std::reference_wrapper<Body const>> bodies) noexcept;
  std::vector<CollisionManifold> narrowPhase(
      std::vector<CollisionManifold> const& manifolds) noexcept;
};
} // namespace redoom::physics
