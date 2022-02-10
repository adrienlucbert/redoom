#pragma once

#include <functional>
#include <vector>

#include <redoom/physics/Body.hh>
#include <redoom/physics/collisions/CollisionManifold.hh>

namespace redoom::physics
{
struct CollisionFilter;

/** This class holds the workflow for detecting collisions.
 * Given a vector of bodies, it returns a vector of collisions happening at
 * the current time.
 */
template <typename Filter>
class CollisionDetector
{
  static_assert(std::is_base_of_v<CollisionFilter, Filter>);

public:
  std::vector<CollisionManifold> operator()(
      std::vector<std::reference_wrapper<Body const>> bodies) noexcept
  {
    auto potential_collisions = this->broadPhase(std::move(bodies));
    auto collisions = this->narrowPhase(potential_collisions);
    return collisions;
  }

private:
  Filter filter;
  std::vector<CollisionManifold> broadPhase(
      std::vector<std::reference_wrapper<Body const>> bodies) noexcept
  {
    return this->filter(std::move(bodies));
  }

  std::vector<CollisionManifold> narrowPhase(
      std::vector<CollisionManifold> const& manifolds) noexcept
  {
    auto result = std::vector<CollisionManifold>{};
    result.reserve(manifolds.size());
    for (auto const& manifold : manifolds) {
      auto aabb_a = *manifold.body_a.get().getGlobalAABB();
      auto aabb_b = *manifold.body_b.get().getGlobalAABB();
      if (aabb_a.intersects(aabb_b))
        result.push_back(manifold);
    }
    return result;
  }
};
} // namespace redoom::physics
