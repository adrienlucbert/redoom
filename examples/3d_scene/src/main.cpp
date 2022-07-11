#include <redoom/Application.hh>
#include <redoom/EntryPoint.hh>
#include <redoom/ecs/Registry.hh>
#include <redoom/ecs/systems/BehaviourSystem.hh>
#include <redoom/ecs/systems/CameraSystem.hh>
#include <redoom/ecs/systems/EventSystem.hh>
#include <redoom/ecs/systems/LightSystem.hh>
#include <redoom/ecs/systems/MeshSystem.hh>
#include <redoom/ecs/systems/ModelSystem.hh>
#include <redoom/ecs/systems/PhysicsDebugSystem.hh>
#include <redoom/ecs/systems/PhysicsWorldSystem.hh>
#include <redoom/graphics/Model.hh>
#include <redoom/graphics/ShaderLibrary.hh>
#include <redoom/graphics/mesh/Quad.hh>

using redoom::ecs::SystemPriority;
using redoom::ecs::systems::BehaviourSystem;
using redoom::ecs::systems::CameraSystem;
using redoom::ecs::systems::EventSystem;
using redoom::ecs::systems::LightSystem;
using redoom::ecs::systems::MeshSystem;
using redoom::ecs::systems::ModelSystem;
using redoom::ecs::systems::PhysicsDebugSystem;
using redoom::ecs::systems::PhysicsWorldSystem;
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

  auto scene_exp = app->loadScene(
      fmt::format("../examples/3d_scene/scenes/{}.yaml", "default"));
  if (!scene_exp) {
    std::cerr << scene_exp.error() << '\n';
    std::abort();
  }
  auto& scene = scene_exp->get();
  auto& registry = scene.getRegistry();

  auto lit_shader_exp = ShaderLibrary::addShader("lit",
      "../examples/3d_scene/shaders/lit/vs.glslx",
      "../examples/3d_scene/shaders/lit/fs.glslx");
  if (!lit_shader_exp) {
    std::cerr << lit_shader_exp.error() << '\n';
    std::abort();
  }
  auto unlit_shader_exp = ShaderLibrary::addShader("unlit",
      "../examples/3d_scene/shaders/unlit/vs.glslx",
      "../examples/3d_scene/shaders/unlit/fs.glslx");
  if (!unlit_shader_exp) {
    std::cerr << unlit_shader_exp.error() << '\n';
    std::abort();
  }

  registry.attachSystem<EventSystem>(SystemPriority{0});
  registry.attachSystem<BehaviourSystem>(SystemPriority{1});
  registry.attachSystem<CameraSystem>(SystemPriority{1});
  registry.attachSystem<LightSystem>(SystemPriority{2});
  registry.attachSystem<PhysicsWorldSystem>(SystemPriority{900});
  registry.attachSystem<PhysicsDebugSystem>(SystemPriority{901});
  registry.attachSystem<MeshSystem>(SystemPriority{1000});
  registry.attachSystem<ModelSystem>(SystemPriority{1000});
  return app;
}
} // namespace redoom
