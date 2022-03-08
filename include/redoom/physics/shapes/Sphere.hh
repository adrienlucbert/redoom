#pragma once

#include <numbers>

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
    , mesh_{tl::nullopt}
  {
  }
  Sphere(Sphere const& b) noexcept = delete;
  Sphere(Sphere&& b) noexcept = default;
  ~Sphere() noexcept override = default;

  Sphere& operator=(Sphere const& rhs) noexcept = delete;
  Sphere& operator=(Sphere&& rhs) noexcept = default;

  void draw(graphics::Program& program) const noexcept override
  {
    if (!this->mesh_.has_value())
      this->mesh_ = graphics::mesh::Sphere{
          this->radius_, 20, 20, {1.0f, 0.0f, 0.0f}, {}, GL_LINE_STRIP};
    this->mesh_->draw(program);
  }

  [[nodiscard]] float getRadius() const noexcept
  {
    return this->radius_;
  }

  [[nodiscard]] AABB const& getAABB() const noexcept override
  {
    return this->aabb_;
  }

  [[nodiscard]] float computeMass(float density) const noexcept override
  {
    return (4.0f / 3.0f) * static_cast<float>(std::numbers::pi)
         * static_cast<float>(std::pow(this->radius_, 3)) * density;
  }

private:
  [[nodiscard]] AABB computeAABB() const noexcept
  {
    return AABB{{-this->radius_, -this->radius_, -this->radius_},
        {this->radius_, this->radius_, this->radius_}};
  }

  float radius_;
  AABB aabb_;
  mutable tl::optional<graphics::mesh::Sphere> mesh_;
};
} // namespace redoom::physics
