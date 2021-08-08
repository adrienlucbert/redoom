#pragma once

#include <array>

#include <GL/glew.h>

namespace redoom::graphics
{
enum class BufferUsage { STATIC = GL_STATIC_DRAW, DYNAMIC = GL_DYNAMIC_DRAW };

template <typename T, std::size_t N>
class Buffer
{
public:
  Buffer(unsigned int ptype,
      std::array<T, N> data,
      unsigned int pcount,
      BufferUsage usage) noexcept
    : id{}
    , type{ptype}
    , count{pcount}
  {
    glCreateBuffers(1, &this->id);
    this->bind();
    glBufferData(
        this->type, sizeof(T) * N, data.data(), static_cast<GLenum>(usage));
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

template <std::size_t N>
struct IndexBuffer : public Buffer<GLuint, N> {
  IndexBuffer(std::array<GLuint, N> data, BufferUsage usage) noexcept
    : Buffer<GLuint, N>{GL_ELEMENT_ARRAY_BUFFER, std::move(data), N, usage}
  {
  }

  void draw() const noexcept
  {
    glDrawElements(GL_TRIANGLES, N, GL_UNSIGNED_INT, nullptr);
  }
};

template <std::size_t N>
struct VertexBuffer : public Buffer<GLfloat, N> {
  VertexBuffer(std::array<GLfloat, N> data,
      unsigned int pcount,
      BufferUsage usage) noexcept
    : Buffer<GLfloat, N>{GL_ARRAY_BUFFER, std::move(data), pcount, usage}
  {
  }

  void draw() const noexcept
  {
    glDrawArrays(GL_TRIANGLES, 0, this->count);
  }
};
} // namespace redoom::graphics
