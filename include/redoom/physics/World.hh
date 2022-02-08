#pragma once

#include <memory>
#include <unordered_map>

#include <redoom/memory/Allocator.hh>

namespace redoom::graphics
{
class Mesh;
class Model;
} // namespace redoom::graphics

namespace redoom::physics
{
class Body;
struct BodyDefinition;

class World
{
public:
  World() noexcept = default;
  World(World const& b) noexcept = delete;
  World(World&& b) noexcept = default;
  ~World() noexcept = default;

  World& operator=(World const& rhs) noexcept = delete;
  World& operator=(World&& rhs) noexcept = default;

  std::shared_ptr<Body> createBody(BodyDefinition def) noexcept;
  std::shared_ptr<Body> createBodyFromModel(
      BodyDefinition def, graphics::Model const& model) noexcept;
  std::shared_ptr<Body> createBodyFromMesh(
      BodyDefinition def, graphics::Mesh const& mesh) noexcept;
  void step(double timestep) noexcept;

  [[nodiscard]] bool getDebugDraw() const noexcept;
  void setDebugDraw(bool draw = true) noexcept;

private:
  bool deleteBody(Body const& body) noexcept;

  bool debug_draw{false};
  unsigned int last_body_id{0};
  memory::Allocator<Body> allocator;
  std::unordered_map<unsigned int, memory::Allocator<Body>::ptr_t> bodies;
};
} // namespace redoom::physics
