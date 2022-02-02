#include <redoom/physics/Body.hh>

#include <redoom/graphics/Model.hh>

namespace redoom::physics
{
Body::Body(World& pworld, unsigned int pid, BodyDefinition def) noexcept
  : world{pworld}
  , id{pid}
  , type{def.type}
  , position{def.position}
  , angle{def.angle}
  , linear_velocity{def.linear_velocity}
  , angular_velocity{def.angular_velocity}
  , has_fixed_rotation{def.has_fixed_rotation}
  , gravity_scale{def.gravity_scale}
{
}

Fixture& Body::createFixture(FixtureDefinition def) noexcept
{
  this->fixtures.emplace_back(*this, std::move(def));
  return this->fixtures.back();
}

Fixture& Body::createFixtureFromMesh(
    FixtureDefinition def, graphics::Mesh const& mesh) noexcept
{
  this->fixtures.push_back(Fixture::fromMesh(*this, std::move(def), mesh));
  return this->fixtures.back();
}

void Body::draw(graphics::Program& program) const noexcept
{
  for (auto const& fixture : this->fixtures)
    fixture.draw(program);
}

World& Body::getWorld() const noexcept
{
  return this->world;
}

unsigned int Body::getId() const noexcept
{
  return this->id;
}

BodyType Body::getType() const noexcept
{
  return this->type;
}

glm::vec3 const& Body::getPosition() const noexcept
{
  return this->position;
}

float Body::getAngle() const noexcept
{
  return this->angle;
}

glm::vec3 const& Body::getLinearVelocity() const noexcept
{
  return this->linear_velocity;
}

float Body::getAngularVelocity() const noexcept
{
  return this->angular_velocity;
}

bool Body::hasFixedRotation() const noexcept
{
  return this->has_fixed_rotation;
}

float Body::getGravityScale() const noexcept
{
  return this->gravity_scale;
}

std::vector<Fixture> const& Body::getFixtures() const noexcept
{
  return this->fixtures;
}
} // namespace redoom::physics
