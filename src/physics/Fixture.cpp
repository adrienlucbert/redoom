#include <redoom/physics/Fixture.hh>

namespace redoom::physics
{
Fixture::Fixture(Body& pbody, Fixture::Definition def) noexcept
  : body{pbody}
  , shape{std::move(def.pshape)}
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
} // namespace redoom::physics
