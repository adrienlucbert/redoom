#pragma once

#include <tl/optional.hpp>

#include <redoom/graphics/mesh/Sphere.hh>
#include <redoom/physics/Shape.hh>

namespace redoom::physics
{
class Sphere : public Shape
{
public:
  explicit Sphere(float pradius) noexcept
    : Shape("Sphere")
    , radius{pradius}
    , mesh{graphics::mesh::Sphere{
          pradius, 20, 20, {1.0f, 0.0f, 0.0f}, {}, GL_LINE_STRIP}}
  {
  }
  Sphere(Sphere const& b) noexcept = delete;
  Sphere(Sphere&& b) noexcept = default;
  ~Sphere() noexcept override = default;

  Sphere& operator=(Sphere const& rhs) noexcept = delete;
  Sphere& operator=(Sphere&& rhs) noexcept = default;

  void draw(graphics::Program& program) const noexcept override
  {
    if (this->mesh.has_value())
      this->mesh.value().draw(program);
  }

  [[nodiscard]] float getRadius() const noexcept
  {
    return this->radius;
  }

private:
  float radius;
  tl::optional<graphics::mesh::Sphere> mesh;
};
} // namespace redoom::physics
