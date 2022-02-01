#pragma once

#include <glm/gtc/type_ptr.hpp>

#include <redoom/graphics/mesh/Cuboid.hh>

namespace redoom::physics
{
class BoundingBox
{
public:
  BoundingBox(glm::vec2 const& px_bounds,
      glm::vec2 const& py_bounds,
      glm::vec2 const& pz_bounds);
  BoundingBox(BoundingBox const& b) noexcept = delete;
  BoundingBox(BoundingBox&& b) noexcept = default;
  ~BoundingBox() noexcept = default;

  BoundingBox& operator=(BoundingBox const& rhs) noexcept = delete;
  BoundingBox& operator=(BoundingBox&& rhs) noexcept = default;

private:
  glm::vec2 x_bounds;
  glm::vec2 y_bounds;
  glm::vec2 z_bounds;
  graphics::mesh::Cuboid mesh;
};
} // namespace redoom::physics
