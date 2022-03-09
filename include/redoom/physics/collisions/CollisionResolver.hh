#pragma once

#include <redoom/physics/collisions/CollisionManifold.hh>

namespace redoom::physics
{
class CollisionResolver
{
public:
  virtual ~CollisionResolver() = default;

  virtual void resolve(CollisionManifold& manifold) const noexcept;
};
} // namespace redoom::physics
