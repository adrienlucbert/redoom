#pragma once

#include <memory>

#include <redoom/physics/Shape.hh>

namespace redoom::physics
{
class Body;

struct FixtureDefinition {
  std::unique_ptr<Shape> shape;
  float friction{0.2f};
  float restitution{0.0f};
  float density{0.0f};
};

class Fixture
{
public:
  Fixture(Body& pbody, FixtureDefinition def) noexcept;

  void draw(graphics::Program& program) const noexcept;

  [[nodiscard]] Body& getBody() const noexcept;
  [[nodiscard]] std::unique_ptr<Shape> const& getShape() const noexcept;
  [[nodiscard]] float getFriction() const noexcept;
  [[nodiscard]] float getRestitution() const noexcept;
  [[nodiscard]] float getDensity() const noexcept;

private:
  Body& body;
  std::unique_ptr<Shape> shape;
  float friction{0.2f};
  float restitution{0.0f};
  float density{0.0f};
};
} // namespace redoom::physics
