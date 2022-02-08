#include <redoom/physics/Fixture.hh>

#include <redoom/graphics/Mesh.hh>
#include <redoom/physics/shapes/Cuboid.hh>

namespace redoom::physics
{
Fixture::Fixture(Body& pbody, FixtureDefinition def) noexcept
  : body{pbody}
  , shape{std::move(def.shape)}
  , local_position{def.local_position}
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

std::shared_ptr<Shape const> Fixture::getShape() const noexcept
{
  return this->shape;
}

glm::vec3 const& Fixture::getLocalPosition() const noexcept
{
  return this->local_position;
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

Fixture Fixture::fromAABB(
    Body& pbody, FixtureDefinition def, AABB const& aabb) noexcept
{
  def.shape = std::make_shared<physics::Cuboid>(
      aabb.upper_bounds.x - aabb.lower_bounds.x,
      aabb.upper_bounds.y - aabb.lower_bounds.y,
      aabb.upper_bounds.z - aabb.lower_bounds.z);
  def.local_position = aabb.getCenter();
  return Fixture{pbody, std::move(def)};
}

Fixture Fixture::fromMesh(
    Body& pbody, FixtureDefinition def, graphics::Mesh const& mesh) noexcept
{
  auto initialized = false;
  auto aabb = AABB{};

  for (auto const& vertex : mesh.getVertices()) {
    auto const& pos = vertex.position;
    if (!initialized) {
      aabb.lower_bounds = {pos.x, pos.y, pos.z};
      aabb.upper_bounds = {pos.x, pos.y, pos.z};
      initialized = true;
    } else {
      if (pos.x < aabb.lower_bounds.x)
        aabb.lower_bounds.x = pos.x;
      if (pos.x > aabb.upper_bounds.x)
        aabb.upper_bounds.x = pos.x;
      if (pos.y < aabb.lower_bounds.y)
        aabb.lower_bounds.y = pos.y;
      if (pos.y > aabb.upper_bounds.y)
        aabb.upper_bounds.y = pos.y;
      if (pos.z < aabb.lower_bounds.z)
        aabb.lower_bounds.z = pos.z;
      if (pos.z > aabb.upper_bounds.z)
        aabb.upper_bounds.z = pos.z;
    }
  }

  return Fixture::fromAABB(pbody, std::move(def), aabb);
}
} // namespace redoom::physics
