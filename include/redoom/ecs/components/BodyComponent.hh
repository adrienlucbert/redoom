#pragma once

#include <memory>

#include <redoom/ecs/Component.hh>
#include <redoom/graphics/Mesh.hh>
#include <redoom/graphics/Model.hh>
#include <redoom/physics/Body.hh>
#include <redoom/physics/World.hh>

namespace redoom::ecs::components
{
struct BodyComponent : public Component<BodyComponent> {
  [[nodiscard]] std::string const& getType() const noexcept override
  {
    static auto const type = std::string{"BodyComponent"};
    return type;
  }

  explicit BodyComponent(
      physics::World& world, physics::BodyDefinition def) noexcept
    : body_{world.createBody(def)}
  {
  }
  explicit BodyComponent(std::shared_ptr<physics::Body> body) noexcept
    : body_{std::move(body)}
  {
  }
  BodyComponent(BodyComponent const&) noexcept = delete;
  BodyComponent(BodyComponent&&) noexcept = default;
  ~BodyComponent() noexcept override = default;

  BodyComponent& operator=(BodyComponent const&) noexcept = delete;
  BodyComponent& operator=(BodyComponent&&) noexcept = delete;

  [[nodiscard]] static BodyComponent fromModel(physics::World& world,
      physics::BodyDefinition def,
      graphics::Model const& model) noexcept
  {
    auto body = world.createBodyFromModel(def, model);
    return BodyComponent{std::move(body)};
  }

  [[nodiscard]] static BodyComponent fromMesh(physics::World& world,
      physics::BodyDefinition def,
      graphics::Mesh const& mesh) noexcept
  {
    auto body = world.createBodyFromMesh(def, mesh);
    return BodyComponent{std::move(body)};
  }

  std::shared_ptr<physics::Body> body_;
};
} // namespace redoom::ecs::components
