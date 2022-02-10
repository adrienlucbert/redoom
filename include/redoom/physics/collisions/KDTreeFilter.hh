#pragma once

#include <redoom/physics/collisions/CollisionFilter.hh>

#include <cassert>

namespace redoom::physics
{
struct KDTreeFilter : public CollisionFilter {
  ~KDTreeFilter() noexcept override = default;

  std::vector<CollisionManifold> operator()(
      std::vector<std::reference_wrapper<Body const>> /*bodies*/) noexcept
      override
  {
    assert("not implemented" == nullptr); // NOLINT
    return {};
  }
};
}; // namespace redoom::physics
