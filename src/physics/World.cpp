#include "redoom/ecs/Entity.hh"
#include <redoom/physics/World.hh>

#include <redoom/physics/Body.hh>

namespace redoom::physics
{
Body& World::createBody(BodyDefinition def) noexcept
{
  auto body_ptr = this->allocator.get(*this, this->last_body_id, def);
  auto const body_id = this->last_body_id++;
  this->bodies.emplace(body_id, std::move(body_ptr));
  return *this->bodies.at(body_id);
}

bool World::deleteBody(Body const& body) noexcept
{
  auto const& body_it = this->bodies.find(body.getId());
  if (body_it == this->bodies.end())
    return false;
  this->bodies.erase(body_it);
  return true;
}

void World::step(double /*timestep*/) noexcept
{
}

bool World::getDebugDraw() const noexcept
{
  return this->debug_draw;
}

void World::setDebugDraw(bool draw) noexcept
{
  this->debug_draw = draw;
}
} // namespace redoom::physics
