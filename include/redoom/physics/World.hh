#pragma once

#include <memory>
#include <mutex>
#include <unordered_map>
#include <vector>

#include <redoom/graphics/Program.hh>
#include <redoom/physics/Body.hh>
#include <redoom/physics/Force.hh>
#include <redoom/physics/collisions/CollisionDetector.hh>
#include <redoom/physics/collisions/CollisionResolver.hh>
#include <redoom/physics/collisions/OctTreeFilter.hh>

namespace redoom::graphics
{
class Mesh;
class Model;
} // namespace redoom::graphics

namespace redoom::physics
{
class World
{
public:
  World() noexcept = default;
  World(World const&) noexcept = delete;
  World(World&&) noexcept = default;
  ~World() noexcept = default;

  World& operator=(World const&) noexcept = delete;
  World& operator=(World&&) noexcept = default;

  std::shared_ptr<Body> createBody(BodyDefinition def) noexcept;
  std::shared_ptr<Body> createBodyFromModel(
      BodyDefinition def, graphics::Model const& model) noexcept;
  std::shared_ptr<Body> createBodyFromMesh(
      BodyDefinition def, graphics::Mesh const& mesh) noexcept;
  void step(double timestep) noexcept;
  void addGlobalConstantForce(Force force) noexcept;

  [[nodiscard]] bool getDebugDraw() const noexcept;
  void setDebugDraw(bool draw = true) noexcept;
  void debugDraw() const noexcept;

private:
  bool deleteBody(Body const& body) noexcept;

  mutable std::unique_ptr<std::mutex> mutex_{std::make_unique<std::mutex>()};
  bool debug_draw_{false};
  unsigned int last_body_id_{0};
  std::unordered_map<unsigned int, Body> bodies_;
  CollisionDetector<OctTreeFilter> collision_detector_;
  CollisionResolver collision_resolver_;
  std::vector<Force> global_constant_forces_;
};
} // namespace redoom::physics
