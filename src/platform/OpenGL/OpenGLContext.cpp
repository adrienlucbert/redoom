#include <redoom/platform/OpenGL/OpenGLContext.hh>

#include <GL/glew.h>
#include <GLFW/glfw3.h>

namespace redoom::platform::OpenGL
{
void OpenGLContext::swapBuffers() noexcept
{
  glfwSwapBuffers(this->native_window);
}

Expected<std::unique_ptr<renderer::RendererContext>> OpenGLContext::create(
    GLFWwindow* pnative_window) noexcept
{
  glewExperimental = GL_TRUE;
  GLenum err = glewInit();
  if (err != GLEW_OK)
    return make_formatted_unexpected(
        "Failed to create renderer context: {}", glewGetErrorString(err));
  return std::unique_ptr<OpenGLContext>{new OpenGLContext{pnative_window}};
}

OpenGLContext::OpenGLContext(GLFWwindow* pnative_window) noexcept
  : native_window{pnative_window}
{
}
} // namespace redoom::platform::OpenGL
