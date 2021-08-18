#pragma once

#include <Utils/Expected.hh>
#include <redoom/renderer/RendererContext.hh>

class GLFWwindow;

namespace redoom::platform::OpenGL
{
class OpenGLContext : public renderer::RendererContext
{
public:
  static Expected<std::unique_ptr<renderer::RendererContext>> create(
      GLFWwindow* native_window) noexcept;

  void swapBuffers() noexcept override;

protected:
  explicit OpenGLContext(GLFWwindow* native_window) noexcept;

private:
  GLFWwindow* native_window;
};
} // namespace redoom::platform::OpenGL
