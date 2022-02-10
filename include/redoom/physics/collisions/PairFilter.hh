#pragma once

#include <redoom/physics/collisions/CollisionFilter.hh>

namespace redoom::physics
{
struct PairFilter : public CollisionFilter {
  ~PairFilter() noexcept override = default;

  std::vector<CollisionManifold> operator()(
      std::vector<std::reference_wrapper<Body const>> bodies) noexcept override
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
};
}; // namespace redoom::physics
