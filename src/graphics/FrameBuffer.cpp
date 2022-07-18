#include <redoom/graphics/FrameBuffer.hh>

#include <iostream>

#include <GL/glew.h>

#include <redoom/graphics/ShaderLibrary.hh>
#include <redoom/graphics/mesh/Quad.hh>

namespace redoom::graphics
{
FrameBuffer::FrameBuffer(int width, int height) noexcept
{
  glGenFramebuffers(1, &this->id_);
  this->bind();
  auto texture_exp = Texture2D::fromData(
      nullptr, width, height, 3, Texture2D::Type::FrameBuffer);
  if (!texture_exp)
    std::cerr << texture_exp.error() << '\n';
  // TODO(alucbert): handle unexpected
  this->texture_ = std::make_unique<Texture2D>(std::move(*texture_exp));
  this->rb_.bind();
  this->rb_.setStorage(GL_DEPTH24_STENCIL8, width, height);
  this->rb_.unbind();
  glFramebufferRenderbuffer(GL_FRAMEBUFFER,
      GL_DEPTH_STENCIL_ATTACHMENT,
      GL_RENDERBUFFER,
      this->rb_.getId());
  glFramebufferTexture2D(GL_FRAMEBUFFER,
      GL_COLOR_ATTACHMENT0,
      GL_TEXTURE_2D,
      this->texture_->getId(),
      0);
  if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE) {
    // TODO(alucbert): handle error
    std::cerr << "Couldn't initialize framebuffer" << '\n';
  }
  this->unbind();
}

FrameBuffer::FrameBuffer(FrameBuffer&& rhs) noexcept
  : id_{rhs.id_}
  , rb_{std::move(rhs.rb_)}
  , texture_{std::move(rhs.texture_)}
{
  rhs.id_ = 0;
}

FrameBuffer::~FrameBuffer() noexcept
{
  if (this->id_ != 0)
    glDeleteFramebuffers(1, &this->id_);
}

FrameBuffer& FrameBuffer::operator=(FrameBuffer&& rhs) noexcept
{
  if (this != &rhs) {
    std::swap(this->id_, rhs.id_);
    this->rb_ = std::move(rhs.rb_);
    this->texture_ = std::move(rhs.texture_);
  }
  return *this;
}

void FrameBuffer::bind() const noexcept
{
  glBindFramebuffer(GL_FRAMEBUFFER, this->id_);
}

void FrameBuffer::unbind() const noexcept // NOLINT
{
  glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

Texture2D const& FrameBuffer::getTexture() const noexcept
{
  return *this->texture_;
}
} // namespace redoom::graphics
