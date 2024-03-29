#pragma once

#include <GL/glew.h>

#include <redoom/graphics/Buffer.hh>

namespace redoom::graphics
{
class VertexArray
{
public:
  VertexArray() noexcept;
  VertexArray(VertexArray const&) noexcept = delete;
  VertexArray(VertexArray&& b) noexcept;
  ~VertexArray() noexcept;

  VertexArray& operator=(VertexArray const&) noexcept = delete;
  VertexArray& operator=(VertexArray&& rhs) noexcept;

  void bind() const noexcept;
  void unbind() const noexcept;

private:
  unsigned int id_;
};
} // namespace redoom::graphics
