#include <redoom/renderer/Window.hh>

#include <redoom/platform/OpenGL/OpenGLWindow.hh>
#include <redoom/renderer/RendererAPI.hh>

namespace redoom::renderer
{
using platform::OpenGL::OpenGLWindow;

Expected<std::unique_ptr<Window>> Window::create(std::string_view title,
    int width,
    int height,
    EventCallback const& event_callback) noexcept
{
  switch (RendererAPI::getAPI()) {
    case RendererAPI::API::OpenGL:
      return OpenGLWindow::create(title, width, height, event_callback);
    case RendererAPI::API::Unknown:
      return tl::make_unexpected("Unsupported renderer API");
  }
}
} // namespace redoom::renderer
