#include <redoom/events/Key.hh>

#include <redoom/Runtime.hh>

namespace redoom::events
{
bool isKeyPressed(Key key) noexcept
{
  auto state = glfwGetKey(
      static_cast<GLFWwindow*>(Runtime::get().getWindow().getNativeWindow()),
      static_cast<int>(key));
  return state == static_cast<int>(Action::PRESS);
}

bool isKeyReleased(Key key) noexcept
{
  return !isKeyPressed(key);
}
} // namespace redoom::events
