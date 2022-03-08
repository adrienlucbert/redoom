#pragma once

#include <tl/optional.hpp>

#include <redoom/graphics/mesh/Quad.hh>
#include <redoom/physics/Shape.hh>

namespace redoom::physics
{
class Quad : public Shape
{
public:
  Quad(float width, float height) noexcept
    : Shape("Quad")
    , width_{width}
    , height_{height}
    , aabb_{this->computeAABB()}
    , mesh_{tl::nullopt}
  {
  }
  Quad(Quad const& b) noexcept = delete;
  Quad(Quad&& b) noexcept = default;
  ~Quad() noexcept override = default;

  Quad& operator=(Quad const& rhs) noexcept = delete;
  Quad& operator=(Quad&& rhs) noexcept = default;

  void draw(graphics::Program& program) const noexcept override
  {
    if (!this->mesh_.has_value())
      this->mesh_ = graphics::mesh::Quad{
          this->width_, this->height_, {1.0f, 0.0f, 0.0f}, {}, GL_LINE_STRIP};
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

  [[nodiscard]] AABB const& getAABB() const noexcept override
  {
    return this->aabb_;
  }

  [[nodiscard]] float computeMass(float density) const noexcept override
  {
    return this->width_ * this->height_ * density;
  }

private:
  [[nodiscard]] AABB computeAABB() const noexcept
  {
    return AABB{{-this->width_ / 2, -this->height_ / 2, 0.0f},
        {this->width_ / 2, this->height_ / 2, 0.0f}};
  }

  float width_;
  float height_;
  AABB aabb_;
  mutable tl::optional<graphics::mesh::Quad> mesh_;
};
} // namespace redoom::physics
