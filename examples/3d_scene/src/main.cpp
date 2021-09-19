#include <redoom/Application.hh>
#include <redoom/EntryPoint.hh>
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
#include <redoom/graphics/Texture.hh>
#include <redoom/graphics/mesh/Torus.hh>

#include <behaviours/DonutBehaviour.hh>
#include <behaviours/FPSCounterBehaviour.hh>

using redoom::ecs::SystemPriority;
using redoom::ecs::behaviours::CameraBehaviour;
using redoom::ecs::components::CameraComponent;
using redoom::ecs::components::MeshComponent;
using redoom::ecs::components::TransformComponent;
using redoom::ecs::systems::BehaviourSystem;
using redoom::ecs::systems::CameraSystem;
using redoom::ecs::systems::EventSystem;
using redoom::ecs::systems::MeshSystem;
using redoom::graphics::ShaderLibrary;
using redoom::graphics::Texture2D;
using redoom::graphics::mesh::Torus;

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
  auto& scene = app->makeScene("default");

  auto exp = ShaderLibrary::addShader("default",
      "../examples/3d_scene/shaders/vs.glslx",
      "../examples/3d_scene/shaders/fs.glslx");
  if (!exp) {
    std::cerr << exp.error() << '\n';
    assert(false);
  }

  auto tex_exp = Texture2D::fromFile("../assets/donut.jpg");
  if (!tex_exp) {
    std::cerr << exp.error() << '\n';
    assert(false);
  }
  auto& tex = *tex_exp;
  auto textures = std::vector<Texture2D>{};
  textures.push_back(std::move(tex));

  // NOLINTNEXTLINE
  auto mesh = std::shared_ptr<Torus>(
      new Torus{1.0f, 0.5f, 100, 100, {1.0f, 1.0f, 1.0f}, std::move(textures)});

  auto& registry = scene.getRegistry();
  auto fps_counter = registry.makeEntity();
  registry.attachComponent<FPSCounterBehaviour>(fps_counter);
  auto camera = registry.makeEntity();
  registry.attachComponent<CameraComponent>(
      camera, graphics::Camera(glm::vec3{0.0f, 0.0f, 3.0f}));
  registry.attachComponent<CameraBehaviour>(camera);
  auto donut = registry.makeEntity();
  registry.attachComponent<DonutBehaviour>(donut);
  registry.attachComponent<MeshComponent>(donut, mesh);
  registry.attachComponent<TransformComponent>(donut,
      TransformComponent(
          {0.0, 0.0, -5.0}, glm::radians(25.0f), {1.0f, 1.0f, 0.0f}));
  registry.attachSystem<EventSystem>(SystemPriority{0});
  registry.attachSystem<BehaviourSystem>(SystemPriority{1});
  registry.attachSystem<CameraSystem>(SystemPriority{1});
  registry.attachSystem<MeshSystem>(SystemPriority{1000});

  return app;
}
} // namespace redoom
