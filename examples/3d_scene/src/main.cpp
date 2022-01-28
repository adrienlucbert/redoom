#include <redoom/Application.hh>
#include <redoom/EntryPoint.hh>
#include <redoom/ecs/Registry.hh>
#include <redoom/ecs/behaviours/CameraBehaviour.hh>
#include <redoom/ecs/components/BehaviourComponent.hh>
#include <redoom/ecs/components/CameraComponent.hh>
#include <redoom/ecs/components/MeshComponent.hh>
#include <redoom/ecs/components/ModelComponent.hh>
#include <redoom/ecs/components/TransformComponent.hh>
#include <redoom/ecs/systems/BehaviourSystem.hh>
#include <redoom/ecs/systems/CameraSystem.hh>
#include <redoom/ecs/systems/EventSystem.hh>
#include <redoom/ecs/systems/MeshSystem.hh>
#include <redoom/ecs/systems/ModelSystem.hh>
#include <redoom/graphics/Model.hh>
#include <redoom/graphics/ShaderLibrary.hh>
#include <redoom/graphics/mesh/Quad.hh>
#include <redoom/serializer/SceneSerializer.hh>

using redoom::SceneSerializer;
using redoom::ecs::SystemPriority;
using redoom::ecs::components::CameraComponent;
using redoom::ecs::components::MeshComponent;
using redoom::ecs::components::ModelComponent;
using redoom::ecs::components::TransformComponent;
using redoom::ecs::systems::BehaviourSystem;
using redoom::ecs::systems::CameraSystem;
using redoom::ecs::systems::EventSystem;
using redoom::ecs::systems::MeshSystem;
using redoom::ecs::systems::ModelSystem;
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
  SceneSerializer::get().registerComponentFactory(
      "ModelComponent", std::make_unique<ModelComponent::Serializer>());

  auto scene_exp = app->loadScene(
      fmt::format("../examples/3d_scene/scenes/{}.yaml", "default"));
  if (!scene_exp) {
    std::cerr << scene_exp.error() << '\n';
    std::abort();
  }
  auto& scene = scene_exp->get();
  auto& registry = scene.getRegistry();

  auto shader_library_exp = ShaderLibrary::addShader("default",
      "../examples/3d_scene/shaders/vs.glslx",
      "../examples/3d_scene/shaders/fs.glslx");
  if (!shader_library_exp) {
    std::cerr << shader_library_exp.error() << '\n';
    std::abort();
  }

  registry.attachSystem<EventSystem>(SystemPriority{0});
  registry.attachSystem<BehaviourSystem>(SystemPriority{1});
  registry.attachSystem<CameraSystem>(SystemPriority{1});
  registry.attachSystem<MeshSystem>(SystemPriority{1000});
  registry.attachSystem<ModelSystem>(SystemPriority{1000});
  return app;
}
} // namespace redoom
