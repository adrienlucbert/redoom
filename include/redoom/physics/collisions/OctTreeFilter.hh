#pragma once

#include <redoom/physics/Body.hh>
#include <redoom/physics/OctTree.hh>
#include <redoom/physics/collisions/CollisionFilter.hh>

namespace redoom::physics
{
struct BodyItemProxy : public BaseOctTreeItemProxy<Body> {
  [[nodiscard]] static AABB getAABB(Body const& item) noexcept
  {
    auto aabb_opt = item.getGlobalAABB();
    if (!aabb_opt.has_value())
      return AABB{};
    return *aabb_opt;
  }
};

class OctTreeFilter : public CollisionFilter
{
public:
  OctTreeFilter() noexcept = default;
  ~OctTreeFilter() noexcept override = default;

  OctTreeFilter(OctTreeFilter const&) noexcept = delete;
  OctTreeFilter(OctTreeFilter&&) noexcept = default;

  OctTreeFilter& operator=(OctTreeFilter const&) noexcept = delete;
  OctTreeFilter& operator=(OctTreeFilter&&) noexcept = default;

  void insert(Body& item) noexcept override
  {
    this->tree_.insert(item);
  }

  void remove(Body const& item) noexcept override
  {
    this->tree_.remove(item);
  }

  void update(
      std::vector<std::reference_wrapper<Body>> moved_items) noexcept override
  {
    this->tree_.update(std::move(moved_items));
  }

  [[nodiscard]] std::vector<CollisionManifold> getPossibleCollisions() noexcept
      override
  {
    auto manifolds = std::vector<CollisionManifold>{};
    auto pairs = this->tree_.getClosePairs();
    manifolds.reserve(pairs.size());
    for (auto& pair : pairs) {
      manifolds.emplace_back(CollisionManifold{pair.first, pair.second, {}});
    }
    return manifolds;
  }

  void debugDraw() const noexcept override
  {
    this->tree_.debugDraw();
  }

private:
  OctTree<Body, BodyItemProxy> tree_;
};
} // namespace redoom::physics
