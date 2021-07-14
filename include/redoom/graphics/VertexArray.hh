#pragma once

#include <GL/glew.h>

namespace redoom::graphics
{
class VertexArray
{
public:
  VertexArray() noexcept;
  VertexArray(VertexArray const& b) noexcept = delete;
  VertexArray(VertexArray&& b) noexcept = default;
  ~VertexArray() noexcept;

  VertexArray& operator=(VertexArray const& rhs) noexcept = delete;
  VertexArray& operator=(VertexArray&& rhs) noexcept = default;

  void bind() const noexcept;
  static void unbind() noexcept;

private:
  unsigned int id{};
};
} // namespace redoom::graphics
