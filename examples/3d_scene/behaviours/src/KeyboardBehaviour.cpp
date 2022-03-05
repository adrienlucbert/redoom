#include <behaviours/extern.hh>

#include <redoom/Application.hh>
#include <redoom/ecs/Behaviour.hh>
#include <redoom/ecs/components/BodyComponent.hh>
#include <redoom/ecs/components/MeshComponent.hh>
#include <redoom/ecs/components/ModelComponent.hh>
#include <redoom/ecs/components/TransformComponent.hh>
#include <redoom/events/Event.hh>
#include <redoom/physics/Body.hh>
#include <redoom/renderer/Renderer.hh>

using redoom::ecs::Behaviour;
using redoom::ecs::Entity;
using redoom::ecs::components::BodyComponent;
using redoom::ecs::components::MeshComponent;
using redoom::ecs::components::ModelComponent;
using redoom::ecs::components::TransformComponent;
using redoom::physics::BodyDefinition;

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
      redoom::Application::get().getCurrentScene().serialize(
          "../examples/3d_scene/scenes/default.yaml");
    }
    // Switch wireframe mode
    if (event.matches({.key = redoom::events::Key::SPACE,
            .action = redoom::events::Action::PRESS})) {
      auto& renderer_api = redoom::renderer::Renderer::getAPI();
      renderer_api.setWireframe(!renderer_api.isWireframe());
      std::cout << "Setting wireframe mode: " << std::boolalpha
                << renderer_api.isWireframe() << '\n';
    }
    // Switch physics debug mode
    if (event.matches({.key = redoom::events::Key::F3,
            .action = redoom::events::Action::PRESS})) {
      auto& registry =
          redoom::Application::get().getCurrentScene().getRegistry();
      auto& world = redoom::Application::get().getCurrentScene().getWorld();

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
    // Switch VSync
    if (event.matches({.key = redoom::events::Key::GRAVE_ACCENT,
            .action = redoom::events::Action::PRESS})) {
      auto& window = redoom::Application::get().getWindow();
      window.setVSync(!window.hasVSync());
      std::cout << "Setting VSync: " << std::boolalpha << window.hasVSync()
                << '\n';
    }

    // Trigger collision detection
    if (event.matches({.key = redoom::events::Key::C,
            .action = redoom::events::Action::PRESS})) {
      auto& world = redoom::Application::get().getCurrentScene().getWorld();
      world.step(0.0);
    }
  }
};

std::unique_ptr<Behaviour> make() noexcept
{
  return std::make_unique<KeyboardBehaviour>();
}
