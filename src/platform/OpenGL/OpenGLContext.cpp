#include <redoom/platform/OpenGL/OpenGLContext.hh>

#include <GL/glew.h>
#include <GLFW/glfw3.h>

namespace redoom::platform::OpenGL
{
void OpenGLContext::swapBuffers() noexcept
{
  glfwSwapBuffers(this->native_window_);
}

Expected<std::unique_ptr<renderer::RendererContext>> OpenGLContext::create(
    GLFWwindow* native_window) noexcept
{
  glewExperimental = GL_TRUE;
  GLenum err = glewInit();
  if (err != GLEW_OK)
    return make_formatted_unexpected(
        "Failed to create renderer context: {}", glewGetErrorString(err));
  try {
    return std::unique_ptr<OpenGLContext>{new OpenGLContext{native_window}};
  } catch (std::exception const& e) {
    return tl::unexpected(e.what());
  }
}

OpenGLContext::OpenGLContext(GLFWwindow* native_window) noexcept
  : native_window_{native_window}
{
}
} // namespace redoom::platform::OpenGL
