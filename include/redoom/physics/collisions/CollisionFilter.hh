#pragma once

#include <list>

#include <redoom/graphics/Program.hh>
#include <redoom/physics/collisions/CollisionManifold.hh>

namespace redoom::physics
{
/** The aim of the collision filter is to perform pruning on a given set of
 * possible collision. This helps reducing the amount of computations needed
 * for collision detection.
 * This filter will be called during the broad phase of the collision detection
 * workflow.
 */
class CollisionFilter
{
public:
  CollisionFilter() noexcept = default;
  virtual ~CollisionFilter() noexcept = default;

  virtual void insert(Body& item) noexcept = 0;
  virtual void remove(Body const& item) noexcept = 0;

  virtual void update(
      std::vector<std::reference_wrapper<Body>> moved_items) noexcept = 0;

  virtual void debugDraw() const noexcept
  {
  }

  [[nodiscard]] virtual std::vector<CollisionManifold>
  getPossibleCollisions() noexcept = 0;
};
} // namespace redoom::physics
