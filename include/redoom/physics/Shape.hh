#pragma once

#include <string_view>

#include <redoom/physics/AABB.hh>

namespace redoom::graphics
{
class Program;
} // namespace redoom::graphics

namespace redoom::physics
{
class Shape
{
public:
  Shape(Shape const&) noexcept = default;
  Shape(Shape&&) noexcept = default;
  virtual ~Shape() noexcept = default;

  Shape& operator=(Shape const&) noexcept = default;
  Shape& operator=(Shape&&) noexcept = default;

  virtual void draw(graphics::Program& program) const noexcept = 0;

  [[nodiscard]] std::string_view getType() const noexcept;
  [[nodiscard]] virtual AABB const& getAABB() const noexcept = 0;
  [[nodiscard]] virtual float computeMass(float density) const noexcept = 0;

protected:
  explicit Shape(std::string_view type) noexcept;

private:
  std::string_view type_;
};
} // namespace redoom::physics
