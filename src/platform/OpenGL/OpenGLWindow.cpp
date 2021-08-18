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
  this->setCursorMode(CursorMode::Normal);

  glfwSetWindowSizeCallback(
      this->window, [](GLFWwindow* /*window*/, int w, int h) {
        renderer::Renderer::getAPI().setViewport({0, 0, w, h});
      });

  glfwSetWindowCloseCallback(this->window, [](GLFWwindow* /*window*/) {
    // TODO(alucbert):
  });

  glfwSetKeyCallback(this->window,
      [](GLFWwindow* /*window*/,
          int /*key*/,
          int /*scancode*/,
          int /*action*/,
          int /*mods*/) {
        // TODO(alucbert):
      });

  glfwSetCharCallback(
      this->window, [](GLFWwindow* /*window*/, unsigned int /*keycode*/) {
        // TODO(alucbert):
      });

  glfwSetMouseButtonCallback(this->window,
      [](GLFWwindow* /*window*/, int /*button*/, int /*action*/, int /*mods*/) {
        // TODO(alucbert):
      });

  glfwSetScrollCallback(this->window,
      [](GLFWwindow* /*window*/, double /*x_offset*/, double /*y_offset*/) {
        // TODO(alucbert):
      });

  glfwSetCursorPosCallback(this->window,
      [](GLFWwindow* /*window*/, double /*x_pos*/, double /*y_pos*/) {
        // TODO(alucbert):
      });
}
} // namespace redoom::platform::OpenGL
