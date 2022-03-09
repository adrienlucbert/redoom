#include <redoom/physics/World.hh>

#include <algorithm>
#include <iostream>

#include <redoom/graphics/Model.hh>
#include <redoom/physics/Body.hh>

namespace redoom::physics
{
std::shared_ptr<Body> World::createBody(BodyDefinition def) noexcept
{
  auto const body_id = this->last_body_id_++;
  auto [it, success] =
      this->bodies_.emplace(body_id, Body{*this, body_id, def});
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
  this->addBodyToCollisionDetector(*body);
  return body;
}

std::shared_ptr<Body> World::createBodyFromMesh(
    BodyDefinition def, graphics::Mesh const& mesh) noexcept
{
  auto body = World::createBody(def);
  body->createFixtureFromMesh({}, mesh);
  this->addBodyToCollisionDetector(*body);
  return body;
}

void World::addBodyToCollisionDetector(Body& body) noexcept
{
  this->collision_detector_.insert(body);
}

bool World::deleteBody(Body const& body) noexcept
{
  auto const& body_it = this->bodies_.find(body.getId());
  if (body_it == this->bodies_.end())
    return false;
  this->collision_detector_.remove(body_it->second);
  this->bodies_.erase(body_it);
  return true;
}

void World::step(double timestep) noexcept
{
  // Apply forces
  for (auto& [_, body] : this->bodies_) {
    if (body.getType() == BodyType::Static) // don't add forces to static bodies
      continue;

    auto forces_sum = glm::vec3{0.0f};

    for (auto const& global_constant_force : this->global_constant_forces_)
      forces_sum += global_constant_force.compute(body.getMass(), timestep);

    for (auto const& constant_force : body.constant_forces_)
      forces_sum += constant_force.compute(body.getMass(), timestep);
    while (!body.forces_.empty()) {
      auto const& force = body.forces_.front();
      forces_sum += force.compute(body.getMass(), timestep);
      body.forces_.pop();
    }
    body.linear_velocity_ += forces_sum;

    if (body.linear_velocity_.x != 0.0f || body.linear_velocity_.y != 0.0f
        || body.linear_velocity_.z != 0.0f)
      body.transform_.setPosition(
          body.transform_.getPosition() + body.linear_velocity_);
  }

  // Check collisions
  auto vbodies = std::vector<std::reference_wrapper<Body>>{};
  vbodies.reserve(this->bodies_.size());
  std::transform(this->bodies_.begin(),
      this->bodies_.end(),
      std::back_inserter(vbodies),
      [](auto& pair) { return std::ref(pair.second); });
  auto collisions = this->collision_detector_.getCollisions(vbodies);
  for (auto const& collision : collisions) {
    // Collision response
    std::cout << "Collision between body " << collision.body_a.get().getId()
              << " and " << collision.body_b.get().getId() << '\n';
  }
}

void World::addGlobalConstantForce(Force force) noexcept
{
  this->global_constant_forces_.push_back(force);
}

bool World::getDebugDraw() const noexcept
{
  return this->debug_draw_;
}

void World::setDebugDraw(bool draw) noexcept
{
  this->debug_draw_ = draw;
}

void World::debugDraw(graphics::Program& program) const noexcept
{
  if (!this->debug_draw_)
    return;
  this->collision_detector_.debugDraw(program);
}
} // namespace redoom::physics
