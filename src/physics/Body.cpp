#include <redoom/physics/Body.hh>

#include <redoom/graphics/Model.hh>
#include <redoom/physics/World.hh>

namespace redoom::physics
{
Body::Body(World& pworld, unsigned int pid, BodyDefinition def) noexcept
  : world{pworld}
  , id{pid}
  , type{def.type}
  , transform{def.transform}
  , linear_velocity{def.linear_velocity}
  , angular_velocity{def.angular_velocity}
  , has_fixed_rotation{def.has_fixed_rotation}
  , gravity_scale{def.gravity_scale}
  , aabb{tl::nullopt}
{
}

Fixture& Body::createFixture(FixtureDefinition def) noexcept
{
  this->fixtures.emplace_back(*this, std::move(def));
  auto& fixture = this->fixtures.back();
  this->updateAABB(fixture);
  return fixture;
}

Fixture& Body::createFixtureFromMesh(
    FixtureDefinition def, graphics::Mesh const& mesh) noexcept
{
  this->fixtures.push_back(Fixture::fromMesh(*this, std::move(def), mesh));
  auto& fixture = this->fixtures.back();
  this->updateAABB(fixture);
  return fixture;
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

BodyTransform const& Body::getTransform() const noexcept
{
  return this->transform;
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

tl::optional<AABB> Body::getGlobalAABB() const noexcept
{
  if (!this->aabb.has_value())
    return tl::nullopt;
  return this->aabb.value() * this->transform.scale + this->transform.position;
}

tl::optional<AABB> const& Body::getLocalAABB() const noexcept
{
  return this->aabb;
}

void Body::updateAABB(Fixture const& fixture) noexcept
{
  auto fixture_aabb = fixture.getShape()->getAABB();
  fixture_aabb += fixture.getLocalPosition();
  if (this->aabb.has_value())
    this->aabb = this->aabb->combine(fixture_aabb);
  else
    this->aabb = fixture_aabb;
}
} // namespace redoom::physics
