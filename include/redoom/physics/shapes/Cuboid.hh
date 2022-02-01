#pragma once

#include <tl/optional.hpp>

#include <redoom/graphics/mesh/Cuboid.hh>
#include <redoom/physics/Shape.hh>

namespace redoom::physics
{
class Cuboid : public Shape
{
public:
  Cuboid(float pwidth, float pheight, float plength) noexcept
    : Shape("Cuboid")
    , width{pwidth}
    , height{pheight}
    , length{plength}
    , mesh{graphics::mesh::Cuboid{
          pwidth, pheight, plength, {1.0f, 0.0f, 0.0f}, {}, GL_LINE_STRIP}}
  {
  }
  Cuboid(Cuboid const& b) noexcept = delete;
  Cuboid(Cuboid&& b) noexcept = default;
  ~Cuboid() noexcept override = default;

  Cuboid& operator=(Cuboid const& rhs) noexcept = delete;
  Cuboid& operator=(Cuboid&& rhs) noexcept = default;

  void draw(graphics::Program& program) const noexcept override
  {
    if (this->mesh.has_value()) {
      this->mesh.value().draw(program);
    }
  }

  [[nodiscard]] float getWidth() const noexcept
  {
    return this->width;
  }

  [[nodiscard]] float getHeight() const noexcept
  {
    return this->height;
  }

  [[nodiscard]] float getLength() const noexcept
  {
    return this->length;
  }

private:
  float width;
  float height;
  float length;
  tl::optional<graphics::mesh::Cuboid> mesh;
};
} // namespace redoom::physics
