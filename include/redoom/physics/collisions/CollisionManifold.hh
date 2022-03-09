#pragma once

#include <functional>
#include <vector>

namespace redoom::physics
{
class Body;

struct CollisionContactPoint {
};

struct CollisionManifold {
  std::reference_wrapper<Body> body_a;
  std::reference_wrapper<Body> body_b;
  std::vector<CollisionContactPoint> contacts;
};
} // namespace redoom::physics
