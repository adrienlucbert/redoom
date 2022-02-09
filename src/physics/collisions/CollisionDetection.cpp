#include <redoom/physics/collisions/CollisionDetection.hh>

#include <iostream>

namespace redoom::physics
{
std::vector<CollisionManifold> CollisionDetection::getCollisions(
    std::vector<std::reference_wrapper<Body const>> bodies) noexcept
{
  auto potential_collisions = this->broadPhase(std::move(bodies));
  auto collisions = this->narrowPhase(potential_collisions);
  return collisions;
}

std::vector<CollisionManifold> CollisionDetection::broadPhase(
    std::vector<std::reference_wrapper<Body const>> bodies) noexcept
{
  auto manifolds = std::vector<CollisionManifold>{};

  if (bodies.size() <= 1)
    return manifolds;

  auto max_size = 0u;
  for (auto i = 1u; i < bodies.size(); ++i)
    max_size += bodies.size() - i;
  manifolds.reserve(max_size);

  for (auto i = 0u; i < bodies.size() - 1; ++i) {
    for (auto j = i + 1; j < bodies.size(); ++j) {
      manifolds.emplace_back(CollisionManifold{bodies[i], bodies[j], {}});
    }
  }
  return manifolds;
}

std::vector<CollisionManifold> CollisionDetection::narrowPhase(
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
} // namespace redoom::physics
