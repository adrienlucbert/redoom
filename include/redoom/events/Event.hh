#pragma once

#include <variant>

#include <redoom/events/Key.hh>

namespace redoom::events
{
struct WindowFocusEvent {
  bool is_focused;
};

struct WindowResizeEvent {
  int width;
  int height;
};

struct WindowCloseEvent {
};

struct KeyEvent {
  Key key{Key::UNKNOWN};
  int scancode{-1};
  Action action{Action::UNKNOWN};
  int mods{static_cast<int>(Mod::UNKNOWN)};

  [[nodiscard]] bool matches(KeyEvent const& event) const noexcept
  {
    if (event.key != Key::UNKNOWN && event.key != this->key)
      return false;
    if (event.scancode != -1 && event.scancode != this->scancode)
      return false;
    if (event.action != Action::UNKNOWN && event.action != this->action)
      return false;
    if (event.mods != static_cast<int>(Mod::UNKNOWN)
        && event.mods != this->mods)
      return false;
    return true;
  }
};

struct CharEvent {
  unsigned int keycode;

  [[nodiscard]] bool matches(CharEvent const& event) const noexcept
  {
    return event.keycode == this->keycode;
  }
};

struct MouseButtonEvent {
  Mouse button{Mouse::UNKNOWN};
  Action action{Action::UNKNOWN};
  int mods{static_cast<int>(Mod::UNKNOWN)};

  [[nodiscard]] bool matches(MouseButtonEvent const& event) const noexcept
  {
    if (event.button != Mouse::UNKNOWN && event.button != this->button)
      return false;
    if (event.action != Action::UNKNOWN && event.action != this->action)
      return false;
    if (event.mods != static_cast<int>(Mod::UNKNOWN)
        && event.mods != this->mods)
      return false;
    return true;
  }
};

struct MouseMoveEvent {
  double x_pos;
  double y_pos;
};

struct ScrollEvent {
  double x_offset;
  double y_offset;
};

using Event = std::variant<WindowFocusEvent,
    WindowResizeEvent,
    WindowCloseEvent,
    KeyEvent,
    CharEvent,
    MouseButtonEvent,
    MouseMoveEvent,
    ScrollEvent>;
} // namespace redoom::events
