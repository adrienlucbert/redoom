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
  Buffer(unsigned int ptype, std::array<T, N> data, BufferUsage usage) noexcept
    : type{ptype}
  {
    glCreateBuffers(1, &this->id);
    this->bind();
    glBufferData(
        this->type, sizeof(T) * N, data.data(), static_cast<GLenum>(usage));
    this->unbind();
  }
  Buffer(Buffer const& b) noexcept = delete;
  Buffer(Buffer&& b) noexcept = default;
  ~Buffer() noexcept
  {
    glDeleteBuffers(1, &this->id);
  }

  Buffer& operator=(Buffer const& rhs) noexcept = delete;
  Buffer& operator=(Buffer&& rhs) noexcept = default;

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

private:
  unsigned int id{};
  unsigned int type;
};

template <std::size_t N>
struct IndexBuffer : public Buffer<GLuint, N> {
  IndexBuffer(std::array<GLuint, N> data, BufferUsage usage) noexcept
    : Buffer<GLuint, N>{GL_ELEMENT_ARRAY_BUFFER, std::move(data), usage}
  {
  }

  void draw() const noexcept
  {
    glDrawElements(GL_TRIANGLES, N, GL_UNSIGNED_INT, nullptr);
  }
};

template <std::size_t N>
struct VertexBuffer : public Buffer<GLfloat, N> {
  VertexBuffer(std::array<GLfloat, N> data, BufferUsage usage) noexcept
    : Buffer<GLfloat, N>{GL_ARRAY_BUFFER, std::move(data), usage}
  {
  }

  void draw() const noexcept
  {
    glDrawArrays(GL_TRIANGLES, 0, N / 4);
  }
};
} // namespace redoom::graphics
