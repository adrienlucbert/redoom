#include <behaviours/extern.hh>

#include <redoom/Runtime.hh>
#include <redoom/ecs/Behaviour.hh>
#include <redoom/ecs/components/BodyComponent.hh>
#include <redoom/ecs/components/MeshComponent.hh>
#include <redoom/ecs/components/ModelComponent.hh>
#include <redoom/ecs/components/TransformComponent.hh>
#include <redoom/events/Event.hh>
#include <redoom/physics/Body.hh>
#include <redoom/physics/Force.hh>
#include <redoom/renderer/Renderer.hh>

using redoom::ecs::Behaviour;
using redoom::ecs::Entity;
using redoom::ecs::components::BehaviourComponent;
using redoom::ecs::components::BodyComponent;
using redoom::ecs::components::MeshComponent;
using redoom::ecs::components::ModelComponent;
using redoom::ecs::components::TransformComponent;
using redoom::physics::BodyDefinition;
using redoom::physics::Force;

struct KeyboardBehaviour : public Behaviour {
  [[nodiscard]] std::string const& getType() const noexcept override
  {
    static auto const type = std::string{"KeyboardBehaviour"};
    return type;
  }

  void onKey(
      Entity /*entity*/, redoom::events::KeyEvent& event) noexcept override
  {
    // Save scene
    if (event.matches({.key = redoom::events::Key::S,
            .action = redoom::events::Action::PRESS,
            .mods = static_cast<int>(redoom::events::Mod::CONTROL)})) {
      std::cout << "Saving current scene" << '\n';
      redoom::Runtime::get().getCurrentScene().serialize(
          "../examples/3d_scene/scenes/default.yaml");
    }
    // Switch wireframe mode
    if (event.matches({.key = redoom::events::Key::SPACE,
            .action = redoom::events::Action::PRESS})) {
      auto& renderer_api = redoom::renderer::Renderer::get().getAPI();
      renderer_api.setWireframe(!renderer_api.isWireframe());
      std::cout << "Setting wireframe mode: " << std::boolalpha
                << renderer_api.isWireframe() << '\n';
    }
    // Switch physics debug mode
    if (event.matches({.key = redoom::events::Key::F3,
            .action = redoom::events::Action::PRESS})) {
      auto& registry = redoom::Runtime::get().getCurrentScene().getRegistry();
      auto& world = redoom::Runtime::get().getCurrentScene().getWorld();

      // Generate missing bodies
      registry.apply<ModelComponent>(
          [&](auto entity, ModelComponent& component) {
            if (!registry.hasComponent<BodyComponent>(entity)) {
              auto transform_opt =
                  registry.getComponent<TransformComponent>(entity);
              if (transform_opt.has_value()) {
                auto def = BodyDefinition{.transform = *transform_opt};
                registry.attachComponent<BodyComponent>(entity,
                    BodyComponent::fromModel(world, def, component.model_));
              }
            }
          });

      registry.apply<MeshComponent>([&](auto entity, MeshComponent& component) {
        if (!registry.hasComponent<BodyComponent>(entity)) {
          auto transform_opt =
              registry.getComponent<TransformComponent>(entity);
          if (transform_opt.has_value()) {
            auto def = BodyDefinition{.transform = *transform_opt};
            registry.attachComponent<BodyComponent>(
                entity, BodyComponent::fromMesh(world, def, *component.mesh_));
          }
        }
      });

      world.setDebugDraw(!world.getDebugDraw());
      std::cout << "Setting physics debug mode: " << std::boolalpha
                << world.getDebugDraw() << '\n';
    }

    // Add gravity
    if (event.matches({.key = redoom::events::Key::G,
            .action = redoom::events::Action::PRESS})) {
      auto& world = redoom::Runtime::get().getCurrentScene().getWorld();
      world.addGlobalConstantForce(
          Force{glm::vec3{0.0f, -0.1f, 0.0f}, Force::Type::Acceleration});
      // TODO(alucbert): -9.81
      std::cout << "Activating gravity" << '\n';
    }

    // Toggle VSync
    if (event.matches({.key = redoom::events::Key::GRAVE_ACCENT,
            .action = redoom::events::Action::PRESS})) {
      auto& window = redoom::Runtime::get().getWindow();
      window.setVSync(!window.hasVSync());
      std::cout << "Setting VSync: " << std::boolalpha << window.hasVSync()
                << '\n';
    }

    // Trigger collision detection
    if (event.matches({.key = redoom::events::Key::C,
            .action = redoom::events::Action::PRESS})) {
      auto& world = redoom::Runtime::get().getCurrentScene().getWorld();
      world.step(0.0);
    }

    // Unfocus window
    if (event.matches({.key = redoom::events::Key::ESCAPE,
            .action = redoom::events::Action::PRESS})) {
      redoom::Runtime::get().getWindow().setCursorMode(
          redoom::renderer::Window::CursorMode::Normal);
    }
  }

  void onMouseButton(Entity /*entity*/,
      redoom::events::MouseButtonEvent& event) noexcept override
  {
    // Focus window
    if (event.matches({.button = redoom::events::Mouse::BUTTON_LEFT,
            .action = redoom::events::Action::PRESS})) {
      redoom::Runtime::get().getWindow().setCursorMode(
          redoom::renderer::Window::CursorMode::Disabled);
    }
  }
};

void serialize(YAML::Emitter& /*out*/, BehaviourComponent const* /*component*/)
{
}

redoom::Expected<std::unique_ptr<Behaviour>> deserialize(
    YAML::Node const& /*node*/,
    redoom::Scene& /*scene*/,
    redoom::ecs::Entity /*entity*/)
{
  return std::make_unique<KeyboardBehaviour>();
}
