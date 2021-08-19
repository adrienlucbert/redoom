#include <redoom/Application.hh>
#include <redoom/events/Key.hh>

namespace redoom::events
{
bool isKeyPressed(Key key) noexcept
{
  auto state = glfwGetKey(static_cast<GLFWwindow*>(
                              Application::get().getWindow().getNativeWindow()),
      static_cast<int>(key));
  return state == static_cast<int>(Action::PRESS);
}

bool isKeyReleased(Key key) noexcept
{
  return !isKeyPressed(key);
}
} // namespace redoom::events
