#pragma once

#include <functional>
#include <vector>

#include <redoom/graphics/Shader.hh>
#include <redoom/physics/Body.hh>
#include <redoom/physics/collisions/CollisionManifold.hh>

namespace redoom::physics
{
class CollisionFilter;

/** This class holds the workflow for detecting collisions.
 * Given a vector of bodies, it returns a vector of collisions happening at
 * the current time.
 */
template <typename Filter>
requires std::is_base_of_v<CollisionFilter, Filter>
class CollisionDetector
{
public:
  CollisionDetector() noexcept = default;
  ~CollisionDetector() noexcept = default;

  CollisionDetector(CollisionDetector const&) noexcept = delete;
  CollisionDetector(CollisionDetector&&) noexcept = default;

  CollisionDetector& operator=(CollisionDetector const&) noexcept = delete;
  CollisionDetector& operator=(CollisionDetector&&) noexcept = default;

  void insert(Body& item) noexcept
  {
    this->filter_.insert(item);
  }

  void remove(Body const& item) noexcept
  {
    this->filter_.remove(item);
  }

  std::vector<CollisionManifold> getCollisions(
      std::vector<std::reference_wrapper<Body>> const& items) noexcept
  {
    auto potential_collisions = this->broadPhase(items);
    auto collisions = this->narrowPhase(potential_collisions);
    return collisions;
  }

  void debugDraw() const noexcept
  {
    this->filter_.debugDraw();
  }

private:
  std::vector<CollisionManifold> broadPhase(
      std::vector<std::reference_wrapper<Body>> const& items) noexcept
  {
    auto moved_items = std::vector<std::reference_wrapper<Body>>{};
    for (auto const& item : items) {
      if (item.get().getTransform().isUpdated()) {
        moved_items.emplace_back(item);
      }
    }
    this->filter_.update(moved_items);
    auto possible_collisions = this->filter_.getPossibleCollisions();
    return possible_collisions;
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

  Filter filter_;
};
} // namespace redoom::physics
