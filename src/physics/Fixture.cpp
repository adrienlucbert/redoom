#include <redoom/physics/Fixture.hh>

#include <redoom/graphics/Mesh.hh>
#include <redoom/physics/shapes/Cuboid.hh>
#include <redoom/physics/shapes/Quad.hh>

namespace redoom::physics
{
Fixture::Fixture(Body& body, FixtureDefinition def) noexcept
  : body_{body}
  , shape_{std::move(def.shape)}
  , local_position_{def.local_position}
  , friction_{def.friction}
  , restitution_{def.restitution}
  , density_{def.density}
  , mass_{this->shape_->computeMass(this->density_)}
{
}

void Fixture::draw() const noexcept
{
  if (this->shape_)
    this->shape_->draw();
}

Body& Fixture::getBody() const noexcept
{
  return this->body_;
}

std::shared_ptr<Shape const> Fixture::getShape() const noexcept
{
  return this->shape_;
}

glm::vec3 const& Fixture::getLocalPosition() const noexcept
{
  return this->local_position_;
}

float Fixture::getFriction() const noexcept
{
  return this->friction_;
}

float Fixture::getRestitution() const noexcept
{
  return this->restitution_;
}

float Fixture::getDensity() const noexcept
{
  return this->density_;
}

float Fixture::getMass() const noexcept
{
  return this->mass_;
}

Fixture Fixture::fromAABB(
    Body& body, FixtureDefinition def, AABB const& aabb) noexcept
{
  if (aabb.getSize().z == 0.0f) {
    def.shape = std::make_shared<physics::Quad>(
        aabb.upper_bounds.x - aabb.lower_bounds.x,
        aabb.upper_bounds.y - aabb.lower_bounds.y);
  } else {
    def.shape = std::make_shared<physics::Cuboid>(
        aabb.upper_bounds.x - aabb.lower_bounds.x,
        aabb.upper_bounds.y - aabb.lower_bounds.y,
        aabb.upper_bounds.z - aabb.lower_bounds.z);
  }
  def.local_position = aabb.getCenter();
  return Fixture{body, std::move(def)};
}

Fixture Fixture::fromMesh(
    Body& body, FixtureDefinition def, graphics::Mesh const& mesh) noexcept
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

  return Fixture::fromAABB(body, std::move(def), aabb);
}
} // namespace redoom::physics
