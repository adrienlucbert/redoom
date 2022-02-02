#include <redoom/physics/Fixture.hh>

#include <redoom/graphics/Mesh.hh>
#include <redoom/physics/shapes/Cuboid.hh>

namespace redoom::physics
{
Fixture::Fixture(Body& pbody, FixtureDefinition def) noexcept
  : body{pbody}
  , shape{std::move(def.shape)}
  , position{def.position}
  , friction{def.friction}
  , restitution{def.restitution}
  , density{def.density}
{
}

void Fixture::draw(graphics::Program& program) const noexcept
{
  if (this->shape)
    this->shape->draw(program);
}

Body& Fixture::getBody() const noexcept
{
  return this->body;
}

std::unique_ptr<Shape> const& Fixture::getShape() const noexcept
{
  return this->shape;
}

glm::vec3 const& Fixture::getPosition() const noexcept
{
  return this->position;
}

float Fixture::getFriction() const noexcept
{
  return this->friction;
}

float Fixture::getRestitution() const noexcept
{
  return this->restitution;
}

float Fixture::getDensity() const noexcept
{
  return this->density;
}

Fixture Fixture::fromBounds(Body& pbody,
    FixtureDefinition def,
    glm::vec2 x_bounds,
    glm::vec2 y_bounds,
    glm::vec2 z_bounds) noexcept
{
  def.shape = std::make_unique<physics::Cuboid>(x_bounds[1] - x_bounds[0],
      y_bounds[1] - y_bounds[0],
      z_bounds[1] - z_bounds[0]);
  def.position = {(x_bounds[0] + x_bounds[1]) / 2,
      (y_bounds[0] + y_bounds[1]) / 2,
      (z_bounds[0] + z_bounds[1]) / 2};
  return Fixture{pbody, std::move(def)};
}

Fixture Fixture::fromMesh(
    Body& pbody, FixtureDefinition def, graphics::Mesh const& mesh) noexcept
{
  auto initialized = false;
  auto x_bounds = glm::vec2{};
  auto y_bounds = glm::vec2{};
  auto z_bounds = glm::vec2{};

  for (auto const& vertex : mesh.getVertices()) {
    auto const& pos = vertex.position;
    if (!initialized) {
      x_bounds = {pos[0], pos[0]};
      y_bounds = {pos[1], pos[1]};
      z_bounds = {pos[2], pos[2]};
      initialized = true;
    } else {
      if (pos.x < x_bounds[0])
        x_bounds[0] = pos.x;
      if (pos.x > x_bounds[1])
        x_bounds[1] = pos.x;
      if (pos.y < y_bounds[0])
        y_bounds[0] = pos.y;
      if (pos.y > y_bounds[1])
        y_bounds[1] = pos.y;
      if (pos.z < z_bounds[0])
        z_bounds[0] = pos.z;
      if (pos.z > z_bounds[1])
        z_bounds[1] = pos.z;
    }
  }

  return Fixture::fromBounds(
      pbody, std::move(def), x_bounds, y_bounds, z_bounds);
}
} // namespace redoom::physics
