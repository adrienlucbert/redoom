#include <redoom/renderer/RendererAPI.hh>

#include <cassert>

#include <redoom/platform/OpenGL/OpenGLRendererAPI.hh>
#include <tl/expected.hpp>

namespace redoom::renderer
{
using platform::OpenGL::OpenGLRendererAPI;

RendererAPI::API RendererAPI::getAPI() noexcept
{
  // NOTE: the choice of API is hardcoded here, but it should not be hardcoded
  // anywhere else
  return API::OpenGL;
}

Expected<std::unique_ptr<RendererAPI>> RendererAPI::create() noexcept
{
  switch (RendererAPI::getAPI()) {
    case RendererAPI::API::OpenGL: return OpenGLRendererAPI::create();
    case RendererAPI::API::Unknown:
      return tl::make_unexpected("Unsupported renderer API");
  }
}
} // namespace redoom::renderer
