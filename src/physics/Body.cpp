#include <redoom/physics/Body.hh>

#include <redoom/graphics/Model.hh>
#include <redoom/physics/World.hh>

namespace redoom::physics
{
BodyTransform::BodyTransform(glm::vec3 position,
    float angle,
    glm::vec3 rotation,
    glm::vec3 scale) noexcept
  : position_{position}
  , angle_{angle}
  , rotation_{rotation}
  , scale_{scale}
{
}

glm::vec3 const& BodyTransform::getPosition() const noexcept
{
  return this->position_;
}
void BodyTransform::setPosition(glm::vec3 position) noexcept
{
  this->position_ = position;
  this->is_updated_ = true;
}
float BodyTransform::getAngle() const noexcept
{
  return this->angle_;
}
void BodyTransform::setAngle(float angle) noexcept
{
  this->angle_ = angle;
  this->is_updated_ = true;
}
glm::vec3 const& BodyTransform::getRotation() const noexcept
{
  return this->rotation_;
}
void BodyTransform::setRotation(glm::vec3 rotation) noexcept
{
  this->rotation_ = rotation;
  this->is_updated_ = true;
}
glm::vec3 const& BodyTransform::getScale() const noexcept
{
  return this->scale_;
}
void BodyTransform::setScale(glm::vec3 scale) noexcept
{
  this->scale_ = scale;
  this->is_updated_ = true;
}
bool BodyTransform::isUpdated() const noexcept
{
  if (this->is_updated_) {
    this->is_updated_ = false;
    return true;
  }
  return false;
}

Body::Body(World& world, unsigned int id, BodyDefinition def) noexcept
  : world_{world}
  , id_{id}
  , type_{def.type}
  , transform_{def.transform}
  , linear_velocity_{def.linear_velocity}
  , angular_velocity_{def.angular_velocity}
  , has_fixed_rotation_{def.has_fixed_rotation}
  , gravity_scale_{def.gravity_scale}
{
}

Fixture& Body::createFixture(FixtureDefinition def) noexcept
{
  this->fixtures_.emplace_back(*this, std::move(def));
  return this->fixtures_.back();
}

Fixture& Body::createFixtureFromMesh(
    FixtureDefinition def, graphics::Mesh const& mesh) noexcept
{
  this->fixtures_.push_back(Fixture::fromMesh(*this, std::move(def), mesh));
  return this->fixtures_.back();
}

void Body::draw(graphics::Program& program) const noexcept
{
  for (auto const& fixture : this->fixtures_)
    fixture.draw(program);
}

World& Body::getWorld() const noexcept
{
  return this->world_;
}

unsigned int Body::getId() const noexcept
{
  return this->id_;
}

BodyType Body::getType() const noexcept
{
  return this->type_;
}

BodyTransform const& Body::getTransform() const noexcept
{
  return this->transform_;
}

glm::vec3 const& Body::getLinearVelocity() const noexcept
{
  return this->linear_velocity_;
}

float Body::getAngularVelocity() const noexcept
{
  return this->angular_velocity_;
}

bool Body::hasFixedRotation() const noexcept
{
  return this->has_fixed_rotation_;
}

float Body::getGravityScale() const noexcept
{
  return this->gravity_scale_;
}

std::vector<Fixture> const& Body::getFixtures() const noexcept
{
  return this->fixtures_;
}

tl::optional<AABB> Body::getGlobalAABB() const noexcept
{
  auto aabb_opt = this->getLocalAABB();
  if (!aabb_opt)
    return tl::nullopt;
  return aabb_opt.value() * this->transform_.getScale()
       + this->transform_.getPosition();
}

tl::optional<AABB> Body::getLocalAABB() const noexcept
{
  auto aabb = tl::optional<AABB>{tl::nullopt};
  for (auto const& fixture : this->fixtures_) {
    auto fixture_aabb = fixture.getShape()->getAABB();
    fixture_aabb += fixture.getLocalPosition();
    if (aabb.has_value())
      aabb = aabb->combine(fixture_aabb);
    else
      aabb = AABB{fixture_aabb};
  }
  return aabb;
}

float Body::getMass() const noexcept
{
  return this->mass_;
}

void Body::addForce(Force force) noexcept
{
  this->forces_.push(force);
}

void Body::addConstantForce(Force force) noexcept
{
  this->constant_forces_.push_back(force);
}
} // namespace redoom::physics
