#include <behaviours/extern.hh>

#include <redoom/Application.hh>
#include <redoom/ecs/Behaviour.hh>
#include <redoom/events/Event.hh>

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
    if (event.matches({.key = redoom::events::Key::S,
            .action = redoom::events::Action::PRESS,
            .mods = static_cast<int>(redoom::events::Mod::CONTROL)})) {
      std::cout << "Saving current scene" << '\n';
      redoom::Application::get().getCurrentScene().serialize(
          "../examples/3d_scene/scenes/default.yaml");
    }
  }
};

std::unique_ptr<Behaviour> make() noexcept
{
  return std::make_unique<KeyboardBehaviour>();
}
