#pragma once

#include <tl/optional.hpp>

#include <redoom/graphics/mesh/Sphere.hh>
#include <redoom/physics/Shape.hh>

namespace redoom::physics
{
class Sphere : public Shape
{
public:
  explicit Sphere(float radius) noexcept
    : Shape("Sphere")
    , radius_{radius}
    , aabb_{this->computeAABB()}
    , mesh_{graphics::mesh::Sphere{
          radius, 20, 20, {1.0f, 0.0f, 0.0f}, {}, GL_LINE_STRIP}}
  {
  }
  Sphere(Sphere const& b) noexcept = delete;
  Sphere(Sphere&& b) noexcept = default;
  ~Sphere() noexcept override = default;

  Sphere& operator=(Sphere const& rhs) noexcept = delete;
  Sphere& operator=(Sphere&& rhs) noexcept = default;

  void draw(graphics::Program& program) const noexcept override
  {
    if (this->mesh_.has_value())
      this->mesh_.value().draw(program);
  }

  [[nodiscard]] float getRadius() const noexcept
  {
    return this->radius_;
  }

  [[nodiscard]] AABB const& getAABB() const noexcept override
  {
    return this->aabb_;
  }

private:
  [[nodiscard]] AABB computeAABB() const noexcept
  {
    return AABB{{-this->radius_, -this->radius_, -this->radius_},
        {this->radius_, this->radius_, this->radius_}};
  }

  float radius_;
  AABB aabb_;
  tl::optional<graphics::mesh::Sphere> mesh_;
};
} // namespace redoom::physics
