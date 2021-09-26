#include <redoom/Application.hh>
#include <redoom/EntryPoint.hh>
#include <redoom/SceneSerializer.hh>
#include <redoom/ecs/Registry.hh>
#include <redoom/ecs/behaviours/CameraBehaviour.hh>
#include <redoom/ecs/components/BehaviourComponent.hh>
#include <redoom/ecs/components/CameraComponent.hh>
#include <redoom/ecs/components/MeshComponent.hh>
#include <redoom/ecs/components/TransformComponent.hh>
#include <redoom/ecs/systems/BehaviourSystem.hh>
#include <redoom/ecs/systems/CameraSystem.hh>
#include <redoom/ecs/systems/EventSystem.hh>
#include <redoom/ecs/systems/MeshSystem.hh>
#include <redoom/graphics/ShaderLibrary.hh>
#include <redoom/graphics/mesh/Quad.hh>

using redoom::SceneSerializer;
using redoom::ecs::SystemPriority;
using redoom::ecs::components::CameraComponent;
using redoom::ecs::components::MeshComponent;
using redoom::ecs::components::TransformComponent;
using redoom::ecs::systems::BehaviourSystem;
using redoom::ecs::systems::CameraSystem;
using redoom::ecs::systems::EventSystem;
using redoom::ecs::systems::MeshSystem;
using redoom::graphics::ShaderLibrary;

namespace redoom
{
struct ExampleApplication : public Application {
  explicit ExampleApplication(ApplicationArguments pargs) noexcept
    : Application("3D Scene example", pargs)
  {
  }
};

std::unique_ptr<Application> createApplication(ApplicationArguments args)
{
  auto app = std::make_unique<ExampleApplication>(args);

  SceneSerializer::get().registerComponentFactory(
      "TransformComponent", std::make_unique<TransformComponent::Serializer>());
  SceneSerializer::get().registerComponentFactory(
      "CameraComponent", std::make_unique<CameraComponent::Serializer>());
  SceneSerializer::get().registerComponentFactory("BehaviourComponent",
      std::make_unique<ecs::components::BehaviourComponent::Serializer>());
  SceneSerializer::get().registerComponentFactory(
      "MeshComponent", std::make_unique<MeshComponent::Serializer>());

  auto scene_exp = app->loadScene(
      fmt::format("../examples/3d_scene/scenes/{}.redoom", "default"));
  if (!scene_exp) {
    std::cerr << scene_exp.error() << '\n';
    std::abort();
  }
  auto& scene = scene_exp->get();
  // auto& scene = app->makeScene("default");
  auto& registry = scene.getRegistry();

  auto shader_library_exp = ShaderLibrary::addShader("default",
      "../examples/3d_scene/shaders/vs.glslx",
      "../examples/3d_scene/shaders/fs.glslx");
  if (!shader_library_exp) {
    std::cerr << shader_library_exp.error() << '\n';
    std::abort();
  }

  // // NOLINTNEXTLINE
  // auto plane_mesh = std::shared_ptr<Quad>(new Quad{10.0f, 10.0f});
  //
  // auto camera = registry.makeEntity();
  // registry.attachComponent<CameraComponent>(
  //     camera, graphics::Camera(glm::vec3{0.0f, 2.0f, 5.0f}));
  // registry.attachComponent<CameraBehaviour>(camera);
  //
  // auto floor = registry.makeEntity();
  // registry.attachComponent<MeshComponent>(floor, plane_mesh);
  // registry.attachComponent<TransformComponent>(floor,
  //     TransformComponent(
  //         {0.0, 0.0, 0.0}, glm::radians(90.0f), {1.0f, 0.0f, 0.0f}));

  // scene.serialize(fmt::format("../examples/3d_scene/scenes/{}.redoom",
  // "default"));

  registry.attachSystem<EventSystem>(SystemPriority{0});
  registry.attachSystem<BehaviourSystem>(SystemPriority{1});
  registry.attachSystem<CameraSystem>(SystemPriority{1});
  registry.attachSystem<MeshSystem>(SystemPriority{1000});
  return app;
}
} // namespace redoom
