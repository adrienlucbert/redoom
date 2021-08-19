#pragma once

#include <variant>

namespace redoom::events
{
struct WindowResizeEvent {
  int width;
  int height;
};

struct WindowCloseEvent {
};

struct KeyEvent {
  int key;
  int scancode;
  int action;
  int mods;
};

struct CharEvent {
  unsigned int keycode;
};

struct MouseButtonEvent {
  int button;
  int action;
  int mods;
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
