#include <redoom/graphics/FrameBufferLibrary.hh>

#include <redoom/graphics/FrameBuffer.hh>

namespace redoom::graphics
{
// NOLINTNEXTLINE
auto FrameBufferLibrary::framebuffers_ =
    std::unordered_map<std::string, FrameBuffer>{};

FrameBuffer& FrameBufferLibrary::addFrameBuffer(
    std::string name, int width, int height) noexcept
{
  auto it = FrameBufferLibrary::framebuffers_.emplace(
      std::move(name), FrameBuffer(width, height));
  return it.first->second;
}

tl::optional<FrameBuffer&> FrameBufferLibrary::getFrameBuffer(
    std::string_view name) noexcept
{
  auto const& framebuffer_it =
      FrameBufferLibrary::framebuffers_.find(name.data());
  if (framebuffer_it == FrameBufferLibrary::framebuffers_.end())
    return tl::nullopt;
  else
    return framebuffer_it->second;
}
} // namespace redoom::graphics
