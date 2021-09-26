#include <redoom/renderer/Window.hh>

#include <cassert>

#include <redoom/platform/OpenGL/OpenGLWindow.hh>
#include <redoom/renderer/RendererAPI.hh>

namespace redoom::renderer
{
using platform::OpenGL::OpenGLWindow;

Expected<std::unique_ptr<Window>> Window::create(
    std::string_view title, int width, int height) noexcept
{
  switch (RendererAPI::getAPI()) {
    case RendererAPI::API::OpenGL:
      return OpenGLWindow::create(title, width, height);
    case RendererAPI::API::Unknown:
      return tl::make_unexpected("Unsupported renderer API");
  }
}
} // namespace redoom::renderer
