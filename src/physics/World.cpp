#include <redoom/physics/World.hh>

#include <algorithm>
#include <iostream>

#include <redoom/graphics/Model.hh>
#include <redoom/physics/Body.hh>

namespace redoom::physics
{
std::shared_ptr<Body> World::createBody(BodyDefinition def) noexcept
{
  auto const body_id = this->last_body_id++;
  auto [it, success] = this->bodies.emplace(body_id, Body{*this, body_id, def});
  auto deleter = [&](Body const* ptr) { this->deleteBody(*ptr); };
  return std::shared_ptr<Body>{&it->second, deleter};
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
  auto vbodies = std::vector<std::reference_wrapper<Body const>>{};
  vbodies.reserve(this->bodies.size());
  std::transform(this->bodies.begin(),
      this->bodies.end(),
      std::back_inserter(vbodies),
      [](auto const& pair) { return std::cref(pair.second); });
  auto collisions = this->collision_detector(std::move(vbodies));
  for (auto const& collision : collisions) {
    std::cout << "Collision between body " << collision.body_a.get().getId()
              << " and " << collision.body_b.get().getId() << '\n';
  }
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
