#include <behaviours/extern.hh>

#include <redoom/Application.hh>
#include <redoom/ecs/Behaviour.hh>
#include <redoom/events/Event.hh>
#include <redoom/renderer/Renderer.hh>

using redoom::ecs::Behaviour;
using redoom::ecs::Entity;

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
    // Switch VSync
    if (event.matches({.key = redoom::events::Key::GRAVE_ACCENT,
            .action = redoom::events::Action::PRESS})) {
      auto& window = redoom::Application::get().getWindow();
      window.setVSync(!window.hasVSync());
      std::cout << "Setting VSync: " << std::boolalpha << window.hasVSync()
                << '\n';
    }
  }
};

std::unique_ptr<Behaviour> make() noexcept
{
  return std::make_unique<KeyboardBehaviour>();
}
