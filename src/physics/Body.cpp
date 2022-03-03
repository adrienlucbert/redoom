#include <redoom/physics/Body.hh>

#include <redoom/graphics/Model.hh>
#include <redoom/physics/World.hh>

namespace redoom::physics
{
BodyTransform::BodyTransform(glm::vec3 pposition,
    float pangle,
    glm::vec3 protation,
    glm::vec3 pscale) noexcept
  : position{pposition}
  , angle{pangle}
  , rotation{protation}
  , scale{pscale}
{
}

glm::vec3 const& BodyTransform::getPosition() const noexcept
{
  return this->position;
}
void BodyTransform::setPosition(glm::vec3 pposition) noexcept
{
  this->position = pposition;
  this->is_updated = true;
}
float BodyTransform::getAngle() const noexcept
{
  return this->angle;
}
void BodyTransform::setAngle(float pangle) noexcept
{
  this->angle = pangle;
  this->is_updated = true;
}
glm::vec3 const& BodyTransform::getRotation() const noexcept
{
  return this->rotation;
}
void BodyTransform::setRotation(glm::vec3 protation) noexcept
{
  this->rotation = protation;
  this->is_updated = true;
}
glm::vec3 const& BodyTransform::getScale() const noexcept
{
  return this->scale;
}
void BodyTransform::setScale(glm::vec3 pscale) noexcept
{
  this->scale = pscale;
  this->is_updated = true;
}
bool BodyTransform::isUpdated() const noexcept
{
  if (this->is_updated) {
    this->is_updated = false;
    return true;
  }
  return false;
}

Body::Body(World& pworld, unsigned int pid, BodyDefinition def) noexcept
  : world{pworld}
  , id{pid}
  , type{def.type}
  , transform{def.transform}
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
  auto aabb_opt = this->getLocalAABB();
  if (!aabb_opt)
    return tl::nullopt;
  return aabb_opt.value() * this->transform.getScale()
       + this->transform.getPosition();
}

tl::optional<AABB> Body::getLocalAABB() const noexcept
{
  auto aabb = tl::optional<AABB>{tl::nullopt};
  for (auto const& fixture : this->fixtures) {
    auto fixture_aabb = fixture.getShape()->getAABB();
    fixture_aabb += fixture.getLocalPosition();
    if (aabb.has_value())
      aabb = aabb->combine(fixture_aabb);
    else
      aabb = AABB{fixture_aabb};
  }
  return aabb;
}
} // namespace redoom::physics
