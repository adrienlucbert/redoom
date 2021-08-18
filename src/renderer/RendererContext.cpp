#include <redoom/renderer/RendererContext.hh>

#include <redoom/platform/OpenGL/OpenGLContext.hh>
#include <redoom/renderer/RendererAPI.hh>

namespace redoom::renderer
{
using platform::OpenGL::OpenGLContext;

Expected<std::unique_ptr<RendererContext>> RendererContext::create(
    void* native_window) noexcept
{
  switch (RendererAPI::getAPI()) {
    case RendererAPI::API::OpenGL:
      return OpenGLContext::create(static_cast<GLFWwindow*>(native_window));
    case RendererAPI::API::Unknown:
      return tl::make_unexpected("Unsupported renderer API");
  }
}
} // namespace redoom::renderer
