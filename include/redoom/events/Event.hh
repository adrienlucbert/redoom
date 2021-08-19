#pragma once

#include <variant>

#include <redoom/events/Key.hh>

namespace redoom::events
{
struct WindowResizeEvent {
  int width;
  int height;
};

struct WindowCloseEvent {
};

struct KeyEvent {
  Key key;
  int scancode;
  Action action;
  Mod mods;
};

struct CharEvent {
  unsigned int keycode;
};

struct MouseButtonEvent {
  Mouse button;
  Action action;
  Mod mods;
};

struct MouseMoveEvent {
  double x_pos;
  double y_pos;
};

struct ScrollEvent {
  double x_offset;
  double y_offset;
};

using Event = std::variant<WindowResizeEvent,
    WindowCloseEvent,
    KeyEvent,
    CharEvent,
    MouseButtonEvent,
    MouseMoveEvent,
    ScrollEvent>;
} // namespace redoom::events
