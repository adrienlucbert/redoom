#pragma once

#include <redoom/physics/Body.hh>
#include <redoom/physics/OctTree.hh>
#include <redoom/physics/collisions/CollisionFilter.hh>

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
  OctTreeFilter() noexcept = default;
  ~OctTreeFilter() noexcept override = default;

  OctTreeFilter(OctTreeFilter const& rhs) noexcept = delete;
  OctTreeFilter(OctTreeFilter&& rhs) noexcept = default;

  OctTreeFilter& operator=(OctTreeFilter const& rhs) noexcept = delete;
  OctTreeFilter& operator=(OctTreeFilter&& rhs) noexcept = default;

  void insert(Body& item) noexcept override
  {
    this->tree.insert(item);
  }

  void remove(Body const& item) noexcept override
  {
    this->tree.remove(item);
  }

  void update(
      std::vector<std::reference_wrapper<Body>> moved_items) noexcept override
  {
    this->tree.update(std::move(moved_items));
  }

  [[nodiscard]] std::vector<CollisionManifold> getPossibleCollisions() noexcept
      override
  {
    auto manifolds = std::vector<CollisionManifold>{};
    auto pairs = this->tree.getClosePairs();
    manifolds.reserve(pairs.size());
    for (auto& pair : pairs) {
      manifolds.emplace_back(CollisionManifold{pair.first, pair.second, {}});
    }
    return manifolds;
  }

  void debugDraw(graphics::Program& program) const noexcept override
  {
    this->tree.debugDraw(program);
  }

private:
  OctTree<Body, BodyItemProxy> tree;
};
} // namespace redoom::physics
