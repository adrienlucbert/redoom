#pragma once

#include <redoom/physics/Body.hh>
#include <redoom/physics/OctTree.hh>
#include <redoom/physics/collisions/CollisionFilter.hh>

#include <cassert>

namespace redoom::physics
{
struct BodyItemProxy : public BaseOctTreeItemProxy<Body> {
  [[nodiscard]] static AABB getAABB(Body const& item) noexcept
  {
    // TODO(alucbert): make this noexcept
    return *item.getGlobalAABB();
  }
};

class OctTreeFilter : public CollisionFilter
{
public:
  ~OctTreeFilter() noexcept override = default;

  // TODO(alucbert): update and insert

  std::vector<CollisionManifold> operator()(
      std::vector<std::reference_wrapper<Body const>> /*bodies*/) noexcept
      override
  {
    // TODO(alucbert):
    return {};
  }

private:
  OctTree<Body, BodyItemProxy> tree;
};
} // namespace redoom::physics
