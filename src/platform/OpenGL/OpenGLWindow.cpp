#include <redoom/platform/OpenGL/OpenGLWindow.hh>

#include <cassert>
#include <cstdlib>
#include <iostream>
#include <unordered_map>

#include <fmt/format.h>

#include <redoom/renderer/Renderer.hh>

namespace redoom::platform::OpenGL
{
// NOLINTNEXTLINE
static auto windows_count = 0u;

namespace detail
{
[[noreturn]] static void GLFWErrorCallback(int error, char const* description)
{
  std::cerr << fmt::format("GLFW Error ({}): {}", error, description) << '\n';
  std::abort();
}
} // namespace detail

OpenGLWindow::~OpenGLWindow() noexcept
{
  glfwDestroyWindow(this->window_);
  --windows_count;

  if (windows_count == 0)
    glfwTerminate();
}

int OpenGLWindow::getWidth() const noexcept
{
  return this->width_;
}

int OpenGLWindow::getHeight() const noexcept
{
  return this->height_;
}

void OpenGLWindow::setVSync(bool has_vsync) noexcept
{
  if (has_vsync)
    glfwSwapInterval(1);
  else
    glfwSwapInterval(0);
  this->has_vsync_ = has_vsync;
}

bool OpenGLWindow::hasVSync() noexcept
{
  return this->has_vsync_;
}

void* OpenGLWindow::getNativeWindow() const noexcept
{
  return this->window_;
}

void OpenGLWindow::setCursorMode(CursorMode mode) noexcept
{
  static auto const opengl_modes = std::unordered_map<CursorMode, int>{
      {CursorMode::Normal, GLFW_CURSOR_NORMAL},
      {CursorMode::Hidden, GLFW_CURSOR_HIDDEN},
      {CursorMode::Disabled, GLFW_CURSOR_DISABLED}};
  glfwSetInputMode(this->window_, GLFW_CURSOR, opengl_modes.at(mode));
}

void OpenGLWindow::setEventCallback(
    renderer::EventCallback const& callback) noexcept
{
  this->event_callback_ = callback;
}

void OpenGLWindow::onUpdate() noexcept
{
  this->context_->swapBuffers();
}

Expected<std::unique_ptr<renderer::Window>> OpenGLWindow::create(
    std::string_view title,
    int width,
    int height,
    renderer::EventCallback const& event_callback) noexcept
{
  if (windows_count == 0) {
    if (glfwInit() == GL_FALSE)
      assert("Could not initialise GLFW" == nullptr);
    glfwSetErrorCallback(&detail::GLFWErrorCallback);
  }

  glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
  glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
  glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
  glfwWindowHint(GLFW_OPENGL_DEBUG_CONTEXT, GLFW_TRUE);
  glfwWindowHint(GLFW_RESIZABLE, GL_FALSE);

  auto* window =
      glfwCreateWindow(width, height, title.data(), nullptr, nullptr);
  if (window == nullptr) {
    glfwTerminate();
    return tl::make_unexpected("Failed to create window");
  }
  ++windows_count;
  glfwMakeContextCurrent(window);

  auto context_exp = renderer::RendererContext::create(window);
  if (!context_exp) {
    glfwTerminate();
    RETURN_UNEXPECTED(context_exp);
  }
  auto& context = *context_exp;

  try {
    return std::unique_ptr<OpenGLWindow>(new OpenGLWindow{
        window, std::move(context), width, height, event_callback});
  } catch (std::exception const& e) {
    return tl::unexpected(e.what());
  }
}

OpenGLWindow::OpenGLWindow(GLFWwindow* window,
    std::unique_ptr<renderer::RendererContext> context,
    int width,
    int height,
    renderer::EventCallback event_callback) noexcept
  : width_{width}
  , height_{height}
  , window_{window}
  , context_{std::move(context)}
  , has_vsync_{true}
  , event_callback_{std::move(event_callback)}
{
  this->setVSync(true);
  // this->setCursorMode(CursorMode::Disabled);

  this->dispatchEvent(events::WindowResizeEvent{this->width_, this->height_});

  glfwSetWindowUserPointer(this->window_, this);

  glfwSetWindowFocusCallback(
      this->window_, [](GLFWwindow* native_window, int is_focused) {
        auto* opengl_window =
            static_cast<OpenGLWindow*>(glfwGetWindowUserPointer(native_window));
        opengl_window->dispatchEvent(
            events::WindowFocusEvent{static_cast<bool>(is_focused)});
      });

  glfwSetWindowSizeCallback(
      this->window_, [](GLFWwindow* native_window, int w, int h) {
        auto* opengl_window =
            static_cast<OpenGLWindow*>(glfwGetWindowUserPointer(native_window));
        opengl_window->dispatchEvent(events::WindowResizeEvent{w, h});
        renderer::Renderer::get().getAPI().setViewport({0, 0, w, h});
      });

  glfwSetWindowCloseCallback(this->window_, [](GLFWwindow* native_window) {
    auto* opengl_window =
        static_cast<OpenGLWindow*>(glfwGetWindowUserPointer(native_window));
    opengl_window->dispatchEvent(events::WindowCloseEvent{});
  });

  glfwSetKeyCallback(this->window_,
      [](GLFWwindow* native_window,
          int key,
          int scancode,
          int action,
          int mods) {
        auto* opengl_window =
            static_cast<OpenGLWindow*>(glfwGetWindowUserPointer(native_window));
        opengl_window->dispatchEvent(
            events::KeyEvent{static_cast<events::Key>(key),
                scancode,
                static_cast<events::Action>(action),
                mods});
      });

  glfwSetCharCallback(
      this->window_, [](GLFWwindow* native_window, unsigned int keycode) {
        auto* opengl_window =
            static_cast<OpenGLWindow*>(glfwGetWindowUserPointer(native_window));
        opengl_window->dispatchEvent(events::CharEvent{keycode});
      });

  glfwSetMouseButtonCallback(this->window_,
      [](GLFWwindow* native_window, int button, int action, int mods) {
        auto* opengl_window =
            static_cast<OpenGLWindow*>(glfwGetWindowUserPointer(native_window));
        opengl_window->dispatchEvent(
            events::MouseButtonEvent{static_cast<events::Mouse>(button),
                static_cast<events::Action>(action),
                mods});
      });

  glfwSetScrollCallback(this->window_,
      [](GLFWwindow* native_window, double x_offset, double y_offset) {
        auto* opengl_window =
            static_cast<OpenGLWindow*>(glfwGetWindowUserPointer(native_window));
        opengl_window->dispatchEvent(events::ScrollEvent{x_offset, y_offset});
      });

  glfwSetCursorPosCallback(
      this->window_, [](GLFWwindow* native_window, double x_pos, double y_pos) {
        auto* opengl_window =
            static_cast<OpenGLWindow*>(glfwGetWindowUserPointer(native_window));
        opengl_window->dispatchEvent(events::MouseMoveEvent{x_pos, y_pos});
      });
}

void OpenGLWindow::dispatchEvent(events::Event const& e) const noexcept
{
  if (this->event_callback_ != nullptr)
    this->event_callback_(e);
}
} // namespace redoom::platform::OpenGL
