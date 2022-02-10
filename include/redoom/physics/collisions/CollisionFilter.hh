#pragma once

#include <redoom/physics/collisions/CollisionManifold.hh>

namespace redoom::physics
{
/** The aim of the collision filter is to perform pruning on a given set of
 * possible collision. This helps reducing the amount of computations needed
 * for collision detection.
 * This filter will be called during the broad phase of the collision detection
 * workflow.
 */
struct CollisionFilter {
  virtual ~CollisionFilter() noexcept = default;

  virtual std::vector<CollisionManifold> operator()(
      std::vector<std::reference_wrapper<Body const>> bodies) noexcept = 0;
};
} // namespace redoom::physics
