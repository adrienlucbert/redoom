#pragma once

#include <redoom/graphics/RenderBuffer.hh>
#include <redoom/graphics/Texture.hh>

namespace redoom::graphics
{
class FrameBuffer
{
public:
  FrameBuffer(int width, int height) noexcept;
  FrameBuffer(FrameBuffer const&) noexcept = delete;
  FrameBuffer(FrameBuffer&& rhs) noexcept;
  ~FrameBuffer() noexcept;

  FrameBuffer& operator=(FrameBuffer const&) noexcept = delete;
  FrameBuffer& operator=(FrameBuffer&& rhs) noexcept;

  void bind() const noexcept;
  void unbind() const noexcept;

  [[nodiscard]] Texture2D const& getTexture() const noexcept;

private:
  unsigned int id_{};
  RenderBuffer rb_{};
  std::unique_ptr<Texture2D> texture_;
};
} // namespace redoom::graphics
