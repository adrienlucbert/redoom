#include <redoom/physics/BoundingBox.hh>

namespace redoom::physics
{
BoundingBox::BoundingBox(glm::vec2 const& px_bounds,
    glm::vec2 const& py_bounds,
    glm::vec2 const& pz_bounds)
  : x_bounds{px_bounds}
  , y_bounds{py_bounds}
  , z_bounds{pz_bounds}
  , mesh{x_bounds[1] - x_bounds[0],
        y_bounds[1] - y_bounds[0],
        z_bounds[1] - z_bounds[0]}
{
}
} // namespace redoom::physics
