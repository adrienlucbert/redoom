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

  void resize(int width, int height) noexcept;

  [[nodiscard]] Texture2D const& getTexture() const noexcept;
  [[nodiscard]] glm::vec2 const& getSize() const noexcept;

private:
  void destroy() noexcept;

  unsigned int id_{};
  RenderBuffer rb_{};
  std::unique_ptr<Texture2D> texture_;
  glm::vec2 const size_;
};
} // namespace redoom::graphics
