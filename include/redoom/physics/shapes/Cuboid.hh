#pragma once

#include <tl/optional.hpp>

#include <redoom/graphics/mesh/Cuboid.hh>
#include <redoom/physics/Shape.hh>

namespace redoom::physics
{
class Cuboid : public Shape
{
public:
  Cuboid(float pheight, float pwidth) noexcept
    : width{pwidth}
    , height{pheight}
  {
  }
  Cuboid(Cuboid const& b) noexcept = delete;
  Cuboid(Cuboid&& b) noexcept = default;
  ~Cuboid() noexcept override = default;

  Cuboid& operator=(Cuboid const& rhs) noexcept = delete;
  Cuboid& operator=(Cuboid&& rhs) noexcept = default;

  void draw(graphics::Program& program) const noexcept override
  {
    if (this->mesh.has_value())
      this->mesh.value().draw(program);
  }

private:
  float width;
  float height;
  tl::optional<graphics::mesh::Cuboid> mesh;
};
} // namespace redoom::physics
