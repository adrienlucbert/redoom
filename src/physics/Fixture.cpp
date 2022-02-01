#include <redoom/physics/Fixture.hh>

namespace redoom::physics
{
Fixture::Fixture(Body& pbody, FixtureDefinition def) noexcept
  : body{pbody}
  , shape{std::move(def.shape)}
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
} // namespace redoom::physics
