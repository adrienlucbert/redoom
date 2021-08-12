#pragma once

#include <string>
#include <vector>

#include <GL/glew.h>

namespace redoom::graphics
{
enum class ShaderDataType {
  Float,
  Float2,
  Float3,
  Float4,
  Mat3,
  Mat4,
  Int,
  Int2,
  Int3,
  Int4,
  Bool
};

struct BufferElement {
  BufferElement(ShaderDataType ptype,
      std::string pname,
      bool pnormalized = false) noexcept;

  ShaderDataType type;
  GLenum gl_type;
  std::string name;
  bool normalized;
  unsigned int size;
  unsigned int components_count;
};

struct BufferLayout : private std::vector<BufferElement> {
  BufferLayout(std::initializer_list<BufferElement> elements) noexcept;

  using std::vector<BufferElement>::begin;
  using std::vector<BufferElement>::end;

  unsigned int stride;
};
} // namespace redoom::graphics
