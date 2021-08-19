#include <redoom/platform/OpenGL/OpenGLWindow.hh>

#include <cassert>
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
  assert(false);
}
} // namespace detail

OpenGLWindow::~OpenGLWindow() noexcept
{
  glfwDestroyWindow(this->window);
  --windows_count;

  if (windows_count == 0)
    glfwTerminate();
}

int OpenGLWindow::getWidth() const noexcept
{
  return this->width;
}

int OpenGLWindow::getHeight() const noexcept
{
  return this->height;
}

void OpenGLWindow::setVSync(bool enable) noexcept
{
  if (enable)
    glfwSwapInterval(1);
  else
    glfwSwapInterval(0);
}

bool OpenGLWindow::hasVSync() noexcept
{
  return this->has_vsync;
}

void* OpenGLWindow::getNativeWindow() const noexcept
{
  return this->window;
}

void OpenGLWindow::setCursorMode(CursorMode mode) noexcept
{
  static auto const opengl_modes = std::unordered_map<CursorMode, int>{
      {CursorMode::Normal, GLFW_CURSOR_NORMAL},
      {CursorMode::Hidden, GLFW_CURSOR_HIDDEN},
      {CursorMode::Disabled, GLFW_CURSOR_DISABLED}};
  glfwSetInputMode(this->window, GLFW_CURSOR, opengl_modes.at(mode));
}

bool OpenGLWindow::pollEvent(events::Event& buffer) noexcept
{
  if (this->events.empty())
    return false;
  buffer = this->events.pop();
  return true;
}

void OpenGLWindow::onUpdate() noexcept
{
  glfwPollEvents();
  this->context->swapBuffers();
}

Expected<std::unique_ptr<renderer::Window>> OpenGLWindow::create(
    std::string_view title, int pwidth, int pheight) noexcept
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

  auto* pwindow =
      glfwCreateWindow(pwidth, pheight, title.data(), nullptr, nullptr);
  if (pwindow == nullptr) {
    glfwTerminate();
    return tl::make_unexpected("Failed to create window");
  }
  ++windows_count;
  glfwMakeContextCurrent(pwindow);

  auto context_exp = renderer::RendererContext::create(pwindow);
  if (!context_exp) {
    glfwTerminate();
    RETURN_UNEXPECTED(context_exp);
  }
  auto& pcontext = *context_exp;

  glEnable(GL_DEPTH_TEST);

  return std::unique_ptr<OpenGLWindow>(
      new OpenGLWindow{pwindow, std::move(pcontext), pwidth, pheight});
}

OpenGLWindow::OpenGLWindow(GLFWwindow* pwindow,
    std::unique_ptr<renderer::RendererContext> pcontext,
    int pwidth,
    int pheight) noexcept
  : width{pwidth}
  , height{pheight}
  , window{pwindow}
  , context{std::move(pcontext)}
  , has_vsync{false}
{
  this->setVSync(true);
  this->setCursorMode(CursorMode::Disabled);
  this->events.push(events::WindowResizeEvent{this->width, this->height});

  glfwSetWindowUserPointer(this->window, &this->events);

  glfwSetWindowSizeCallback(
      this->window, [](GLFWwindow* native_window, int w, int h) {
        auto* event_queue = static_cast<events::EventQueue*>(
            glfwGetWindowUserPointer(native_window));
        event_queue->push(events::WindowResizeEvent{w, h});

        renderer::Renderer::getAPI().setViewport({0, 0, w, h});
      });

  glfwSetWindowCloseCallback(this->window, [](GLFWwindow* native_window) {
    auto* event_queue = static_cast<events::EventQueue*>(
        glfwGetWindowUserPointer(native_window));
    event_queue->push(events::WindowCloseEvent{});
  });

  glfwSetKeyCallback(this->window,
      [](GLFWwindow* native_window,
          int key,
          int scancode,
          int action,
          int mods) {
        auto* event_queue = static_cast<events::EventQueue*>(
            glfwGetWindowUserPointer(native_window));
        event_queue->push(events::KeyEvent{static_cast<events::Key>(key),
            scancode,
            static_cast<events::Action>(action),
            static_cast<events::Mod>(mods)});
      });

  glfwSetCharCallback(
      this->window, [](GLFWwindow* native_window, unsigned int keycode) {
        auto* event_queue = static_cast<events::EventQueue*>(
            glfwGetWindowUserPointer(native_window));
        event_queue->push(events::CharEvent{keycode});
      });

  glfwSetMouseButtonCallback(this->window,
      [](GLFWwindow* native_window, int button, int action, int mods) {
        auto* event_queue = static_cast<events::EventQueue*>(
            glfwGetWindowUserPointer(native_window));
        event_queue->push(
            events::MouseButtonEvent{static_cast<events::Mouse>(button),
                static_cast<events::Action>(action),
                static_cast<events::Mod>(mods)});
      });

  glfwSetScrollCallback(this->window,
      [](GLFWwindow* native_window, double x_offset, double y_offset) {
        auto* event_queue = static_cast<events::EventQueue*>(
            glfwGetWindowUserPointer(native_window));
        event_queue->push(events::ScrollEvent{x_offset, y_offset});
      });

  glfwSetCursorPosCallback(
      this->window, [](GLFWwindow* native_window, double x_pos, double y_pos) {
        auto* event_queue = static_cast<events::EventQueue*>(
            glfwGetWindowUserPointer(native_window));
        event_queue->push(events::MouseMoveEvent{x_pos, y_pos});
      });
}
} // namespace redoom::platform::OpenGL
