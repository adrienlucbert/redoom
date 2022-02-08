#include <redoom/physics/World.hh>

#include <redoom/graphics/Model.hh>
#include <redoom/physics/Body.hh>

namespace redoom::physics
{
std::shared_ptr<Body> World::createBody(BodyDefinition def) noexcept
{
  auto body_ptr = this->allocator.get(*this, this->last_body_id, def);
  auto const body_id = this->last_body_id++;
  auto [it, success] = this->bodies.emplace(body_id, std::move(body_ptr));
  return std::shared_ptr<Body>{
      it->second.get(), [&](Body const* ptr) { this->deleteBody(*ptr); }};
}

std::shared_ptr<Body> World::createBodyFromModel(
    BodyDefinition def, graphics::Model const& model) noexcept
{
  auto body = World::createBody(def);
  for (auto const& mesh : model.getMeshes()) {
    body->createFixtureFromMesh({}, mesh);
  }
  return body;
}

std::shared_ptr<Body> World::createBodyFromMesh(
    BodyDefinition def, graphics::Mesh const& mesh) noexcept
{
  auto body = World::createBody(def);
  body->createFixtureFromMesh({}, mesh);
  return body;
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
