#include <redoom/ecs/systems/EventSystem.hh>

#include <cstdlib>
#include <iostream>
#include <type_traits>
#include <variant>

#include <redoom/Application.hh>
#include <redoom/ecs/components/BehaviourComponent.hh>
#include <redoom/events/Event.hh>

namespace redoom::ecs::systems
{
namespace details
{
template <typename EventT, typename Handler, typename... Args>
void dispatchEvent(
    UpdateContext& context, Handler handler, EventT& event, Args&&... args)
{
  context.getComponentManager().apply<components::BehaviourComponent>(
      [&](auto entity, auto& component) {
        auto behaviour_exp = component.template get();
        if (!behaviour_exp)
          std::cerr << behaviour_exp.error() << '\n';
        else {
          auto& behaviour = *behaviour_exp;
          std::invoke(handler,
              behaviour.get(),
              entity,
              event,
              std::forward<Args>(args)...);
        }
      });
}
} // namespace details

void EventSystem::update(UpdateContext& context) noexcept
{
  auto event_buffer = events::Event{};
  while (Application::get().pollEvent(event_buffer)) {
    try {
      std::visit(
          [&](auto&& event) {
            using T = std::remove_cvref_t<decltype(event)>;
            if constexpr (std::is_same_v<T, events::WindowFocusEvent>)
              details::dispatchEvent(context, &Behaviour::onWindowFocus, event);
            else if constexpr (std::is_same_v<T, events::WindowResizeEvent>)
              details::dispatchEvent(
                  context, &Behaviour::onWindowResize, event);
            else if constexpr (std::is_same_v<T, events::WindowCloseEvent>)
              details::dispatchEvent(context, &Behaviour::onWindowClose, event);
            else if constexpr (std::is_same_v<T, events::KeyEvent>)
              details::dispatchEvent(context, &Behaviour::onKey, event);
            else if constexpr (std::is_same_v<T, events::CharEvent>)
              details::dispatchEvent(context, &Behaviour::onChar, event);
            else if constexpr (std::is_same_v<T, events::MouseButtonEvent>)
              details::dispatchEvent(context, &Behaviour::onMouseButton, event);
            else if constexpr (std::is_same_v<T, events::MouseMoveEvent>)
              details::dispatchEvent(context, &Behaviour::onMouseMove, event);
            else if constexpr (std::is_same_v<T, events::ScrollEvent>)
              details::dispatchEvent(context, &Behaviour::onScroll, event);
            else
              assert("Unknown event type" == nullptr);
          },
          event_buffer);
    } catch (std::bad_variant_access const&) {
      std::abort();
    }
  }
}
} // namespace redoom::ecs::systems
