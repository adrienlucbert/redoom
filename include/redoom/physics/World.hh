#pragma once

#include <unordered_map>

#include <redoom/memory/Allocator.hh>

namespace redoom::graphics
{
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

  Body& createBody(BodyDefinition def) noexcept;
  Body& createBodyFromModel(
      BodyDefinition def, graphics::Model const& model) noexcept;
  bool deleteBody(Body const& body) noexcept;
  void step(double timestep) noexcept;

  [[nodiscard]] bool getDebugDraw() const noexcept;
  void setDebugDraw(bool draw = true) noexcept;

private:
  bool debug_draw{false};
  unsigned int last_body_id{0};
  memory::Allocator<Body> allocator;
  std::unordered_map<unsigned int, memory::Allocator<Body>::ptr_t> bodies;
};
} // namespace redoom::physics
