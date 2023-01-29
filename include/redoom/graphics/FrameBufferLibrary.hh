#pragma once

#include <tl/optional.hpp>

#include <Utils/Expected.hh>
#include <redoom/graphics/FrameBuffer.hh>

namespace redoom::graphics
{
class FrameBufferLibrary
{
public:
  static FrameBuffer& addFrameBuffer(
      std::string name, int width, int height) noexcept;
  static tl::optional<FrameBuffer&> getFrameBuffer(
      std::string_view name) noexcept;

private:
  static std::unordered_map<std::string, FrameBuffer> framebuffers_; // NOLINT
};
} // namespace redoom::graphics
