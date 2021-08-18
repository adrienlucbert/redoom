#pragma once

#include <GL/glew.h>
#include <fmt/core.h>

#include <Utils/Concepts.hpp>
#include <redoom/graphics/BufferLayout.hh>
#include <redoom/graphics/Vertex.hh>

namespace redoom::graphics
{
enum class BufferUsage { STATIC = GL_STATIC_DRAW, DYNAMIC = GL_DYNAMIC_DRAW };

template <typename T>
class Buffer
{
public:
  Buffer(unsigned int ptype,
      T const* data,
      unsigned int pcount,
      BufferUsage usage) noexcept
    : id{}
    , type{ptype}
    , count{pcount}
  {
    glCreateBuffers(1, &this->id);
    this->bind();
    glBufferData(
        this->type, sizeof(T) * this->count, data, static_cast<GLenum>(usage));
    this->unbind();
  }
  Buffer(Buffer const& b) noexcept = delete;
  Buffer(Buffer&& b) noexcept
    : id{b.id}
    , type{b.type}
    , count{b.count}
  {
    b.id = 0;
  }
  virtual ~Buffer() noexcept
  {
    if (this->id != 0)
      glDeleteBuffers(1, &this->id);
  }

  Buffer& operator=(Buffer const& rhs) noexcept = delete;
  Buffer& operator=(Buffer&& rhs) noexcept
  {
    if (this != &rhs) {
      std::swap(this->id, rhs.id);
      this->type = rhs.type;
      this->count = rhs.count;
    }
    return *this;
  }

  [[nodiscard]] unsigned int getId() const noexcept
  {
    return this->id;
  }

  void bind() const noexcept
  {
    glBindBuffer(this->type, this->id);
  }
  void unbind() const noexcept
  {
    glBindBuffer(this->type, 0);
  }

  virtual void draw(GLenum /*topology*/) const noexcept
  {
    assert("This buffer is not drawable" == nullptr);
  }

protected:
  unsigned int id;
  unsigned int type;
  unsigned int count;
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

  void draw(GLenum topology) const noexcept override
  {
    glDrawElements(
        topology, static_cast<GLsizei>(this->count), GL_UNSIGNED_INT, nullptr);
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
      glVertexAttribPointer(index,
          static_cast<GLint>(element.components_count),
          element.gl_type,
          element.normalized ? GL_TRUE : GL_FALSE,
          static_cast<GLint>(layout.stride),
          reinterpret_cast<void*>(offset)); // NOLINT
      glEnableVertexAttribArray(index);
      offset += element.size;
      ++index;
    }
    this->unbind();
  }

  void draw(GLenum topology) const noexcept override
  {
    glDrawArrays(topology, 0, static_cast<GLsizei>(this->count));
  }
};
} // namespace redoom::graphics
