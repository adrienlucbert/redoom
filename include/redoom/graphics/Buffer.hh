#pragma once

#include <GL/glew.h>
#include <fmt/core.h>

#include <Utils/Concepts.hpp>
#include <redoom/graphics/BufferLayout.hh>
#include <redoom/graphics/Vertex.hh>
#include <redoom/renderer/Renderer.hh>

namespace redoom::graphics
{
enum class BufferUsage { STATIC = GL_STATIC_DRAW, DYNAMIC = GL_DYNAMIC_DRAW };

template <typename T>
class Buffer
{
public:
  Buffer(unsigned int type,
      T const* data,
      unsigned int count,
      BufferUsage usage) noexcept
    : id_{}
    , type_{type}
    , count_{count}
  {
    glCreateBuffers(1, &this->id_);
    this->bind();
    glBufferData(this->type_,
        sizeof(T) * this->count_,
        data,
        static_cast<GLenum>(usage));
    this->unbind();
  }
  Buffer(Buffer const&) noexcept = delete;
  Buffer(Buffer&& rhs) noexcept
    : id_{rhs.id_}
    , type_{rhs.type_}
    , count_{rhs.count_}
  {
    rhs.id_ = 0;
  }
  virtual ~Buffer() noexcept
  {
    if (this->id_ != 0)
      glDeleteBuffers(1, &this->id_);
  }

  Buffer& operator=(Buffer const&) noexcept = delete;
  Buffer& operator=(Buffer&& rhs) noexcept
  {
    if (this != &rhs) {
      std::swap(this->id_, rhs.id_);
      this->type_ = rhs.type_;
      this->count_ = rhs.count_;
    }
    return *this;
  }

  [[nodiscard]] unsigned int getId() const noexcept
  {
    return this->id_;
  }

  void bind() const noexcept
  {
    glBindBuffer(this->type_, this->id_);
  }
  void unbind() const noexcept
  {
    glBindBuffer(this->type_, 0);
  }

  virtual void draw() const noexcept
  {
    assert("This buffer is not drawable" == nullptr);
  }

protected:
  unsigned int id_;
  unsigned int type_;
  unsigned int count_;
};

template <concepts::ContiguousContainer<GLuint> Container>
struct IndexBuffer : public Buffer<GLuint> {
  IndexBuffer(Container const& data, BufferUsage usage) noexcept
    : Buffer<GLuint>{GL_ELEMENT_ARRAY_BUFFER,
        data.data(),
        static_cast<unsigned int>(data.size()),
        usage}
  {
  }
  ~IndexBuffer() noexcept override = default;

  IndexBuffer(IndexBuffer const&) noexcept = delete;
  IndexBuffer(IndexBuffer&&) noexcept = default;

  IndexBuffer& operator=(IndexBuffer const&) noexcept = delete;
  IndexBuffer& operator=(IndexBuffer&&) noexcept = default;

  void draw() const noexcept override
  {
    glDrawElements(renderer::Renderer::get().getTopology(),
        static_cast<GLsizei>(this->count_),
        GL_UNSIGNED_INT,
        nullptr);
  }
};

template <concepts::ContiguousContainer<Vertex> Container>
struct VertexBuffer : public Buffer<Vertex> {
  VertexBuffer(Container const& data, BufferUsage usage) noexcept
    : Buffer<Vertex>{GL_ARRAY_BUFFER,
        data.data(),
        static_cast<unsigned int>(data.size()),
        usage}
  {
  }
  ~VertexBuffer() noexcept override = default;

  VertexBuffer(VertexBuffer const&) noexcept = delete;
  VertexBuffer(VertexBuffer&&) noexcept = default;

  VertexBuffer& operator=(VertexBuffer const&) noexcept = delete;
  VertexBuffer& operator=(VertexBuffer&&) noexcept = default;

  void setLayout(BufferLayout const& layout) const noexcept
  {
    auto index = 0u;
    auto offset = 0u;
    this->bind();
    for (auto const& element : layout) {
      glEnableVertexAttribArray(index);
      glVertexAttribPointer(index,
          static_cast<GLint>(element.components_count),
          element.gl_type,
          element.normalized ? GL_TRUE : GL_FALSE,
          static_cast<GLint>(layout.stride),
          reinterpret_cast<void*>(offset)); // NOLINT
      offset += element.size;
      ++index;
    }
    this->unbind();
  }

  void draw() const noexcept override
  {
    glDrawArrays(renderer::Renderer::get().getTopology(),
        0,
        static_cast<GLsizei>(this->count_));
  }
};
} // namespace redoom::graphics
