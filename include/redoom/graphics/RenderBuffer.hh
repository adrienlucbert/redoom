#pragma once

#include <utility>

#include <GL/glew.h>

namespace redoom::graphics
{
class RenderBuffer
{
public:
  RenderBuffer() noexcept
  {
    glGenRenderbuffers(1, &this->id_);
  }
  RenderBuffer(RenderBuffer const&) noexcept = delete;
  RenderBuffer(RenderBuffer&& rhs) noexcept
    : id_{rhs.id_}
  {
    rhs.id_ = 0;
  }
  ~RenderBuffer() noexcept
  {
    if (this->id_ != 0)
      glDeleteRenderbuffers(1, &this->id_);
  }

  RenderBuffer& operator=(RenderBuffer const&) noexcept = delete;
  RenderBuffer& operator=(RenderBuffer&& rhs) noexcept
  {
    if (this != &rhs) {
      std::swap(this->id_, rhs.id_);
    }
    return *this;
  }

  void bind() const noexcept
  {
    glBindRenderbuffer(GL_RENDERBUFFER, this->id_);
  }
  void unbind() const noexcept // NOLINT
  {
    glBindRenderbuffer(GL_RENDERBUFFER, 0);
  }

  void setStorage(GLenum format, int width, int height) const noexcept // NOLINT
  {
    glRenderbufferStorage(GL_RENDERBUFFER, format, width, height);
  }

  [[nodiscard]] unsigned int getId() const noexcept
  {
    return this->id_;
  }

private:
  unsigned int id_{};
};
} // namespace redoom::graphics
