#include <redoom/platform/OpenGL/OpenGLRendererAPI.hh>

#include <GL/glew.h>
#include <GLFW/glfw3.h>

namespace redoom::platform::OpenGL
{
bool OpenGLRendererAPI::isWireframe() const noexcept
{
  return this->is_wireframe;
}

void OpenGLRendererAPI::setWireframe(bool pis_wireframe) noexcept
{
  glPolygonMode(GL_FRONT_AND_BACK, pis_wireframe ? GL_LINE : GL_FILL);
  this->is_wireframe = pis_wireframe;
}

void OpenGLRendererAPI::setViewport(
    renderer::RendererViewport viewport) noexcept
{
  glViewport(viewport.x, viewport.y, viewport.width, viewport.height);
}

void OpenGLRendererAPI::setClearColor(const glm::vec4& color) noexcept
{
  glClearColor(color.r, color.g, color.b, color.a); // NOLINT
}

void OpenGLRendererAPI::clear() noexcept
{
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

Expected<std::unique_ptr<renderer::RendererAPI>>
OpenGLRendererAPI::create() noexcept
{
  try {
    return std::unique_ptr<OpenGLRendererAPI>(new OpenGLRendererAPI{});
  } catch (std::exception const& e) {
    return tl::unexpected(e.what());
  }
}
} // namespace redoom::platform::OpenGL
