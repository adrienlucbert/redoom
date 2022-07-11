#pragma once

#include <tl/optional.hpp>

#include <redoom/graphics/mesh/Cuboid.hh>
#include <redoom/physics/Shape.hh>

namespace redoom::physics
{
class Cuboid : public Shape
{
public:
  Cuboid(float width, float height, float length) noexcept
    : Shape("Cuboid")
    , width_{width}
    , height_{height}
    , length_{length}
    , aabb_{this->computeAABB()}
    , mesh_{tl::nullopt}
  {
  }
  Cuboid(Cuboid const&) noexcept = delete;
  Cuboid(Cuboid&&) noexcept = default;
  ~Cuboid() noexcept override = default;

  Cuboid& operator=(Cuboid const&) noexcept = delete;
  Cuboid& operator=(Cuboid&&) noexcept = default;

  void draw(graphics::Program& program) const noexcept override
  {
    if (!this->mesh_.has_value())
      this->mesh_ = graphics::mesh::Cuboid{
          this->width_, this->height_, this->length_, {}, GL_LINE_STRIP};
    this->mesh_->draw(program);
  }

  [[nodiscard]] float getWidth() const noexcept
  {
    return this->width_;
  }

  [[nodiscard]] float getHeight() const noexcept
  {
    return this->height_;
  }

  [[nodiscard]] float getLength() const noexcept
  {
    return this->length_;
  }

  [[nodiscard]] AABB const& getAABB() const noexcept override
  {
    return this->aabb_;
  }

  [[nodiscard]] float computeMass(float density) const noexcept override
  {
    return this->width_ * this->height_ * this->length_ * density;
  }

private:
  [[nodiscard]] AABB computeAABB() const noexcept
  {
    return AABB{{-this->width_ / 2, -this->height_ / 2, -this->length_ / 2},
        {this->width_ / 2, this->height_ / 2, this->length_ / 2}};
  }

  float width_;
  float height_;
  float length_;
  AABB aabb_;
  mutable tl::optional<graphics::mesh::Cuboid> mesh_;
};
} // namespace redoom::physics
