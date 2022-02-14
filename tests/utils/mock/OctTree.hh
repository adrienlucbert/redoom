#pragma once

#include <redoom/physics/AABB.hh>
#include <redoom/physics/OctTree.hh>

namespace mock
{
struct Item {
  redoom::physics::AABB bounding_box;
};

struct ItemProxy : public redoom::physics::BaseOctTreeItemProxy<Item> {
  [[nodiscard]] static redoom::physics::AABB getAABB(Item const& item) noexcept
  {
    return item.bounding_box;
  }
};
} // namespace mock
