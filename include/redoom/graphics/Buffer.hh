#pragma once

#include <GL/glew.h>

#include <Utils/Concepts.hpp>
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
  ~Buffer() noexcept
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

  void draw() const noexcept
  {
    glDrawElements(GL_TRIANGLES,
        static_cast<GLsizei>(this->count),
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

  void linkAttrib(
      GLuint layout, GLint size, GLenum vtype, GLsizei offset) const noexcept
  {
    this->bind();
    glVertexAttribPointer(layout,
        size,
        vtype,
        GL_FALSE,
        sizeof(Vertex),
        reinterpret_cast<void*>(offset)); // NOLINT
    glEnableVertexAttribArray(layout);
    this->unbind();
  }

  void draw() const noexcept
  {
    glDrawArrays(GL_TRIANGLES, 0, static_cast<GLsizei>(this->count));
  }
};
} // namespace redoom::graphics
