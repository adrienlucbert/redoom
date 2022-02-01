#pragma once

#include <memory>

#include <redoom/physics/Shape.hh>

namespace redoom::physics
{
class Body;

class Fixture
{
public:
  struct Definition {
    std::unique_ptr<Shape> pshape;
    float friction{0.2f};
    float restitution{0.0f};
    float density{0.0f};
  };

  Fixture(Body& pbody, Definition def) noexcept;

  void draw(graphics::Program& program) const noexcept;

private:
  Body& body;
  std::unique_ptr<Shape> shape;
  float friction{0.2f};
  float restitution{0.0f};
  float density{0.0f};
};
} // namespace redoom::physics
