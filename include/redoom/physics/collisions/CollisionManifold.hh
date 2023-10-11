#pragma once

#include <functional>
#include <vector>

#include <redoom/physics/collisions/CollisionContactPoint.hh>

namespace redoom::physics
{
class Body;

struct CollisionManifold {
  std::reference_wrapper<Body> body_a;
  std::reference_wrapper<Body> body_b;
  std::vector<CollisionContactPoint> contacts;
};
} // namespace redoom::physics
