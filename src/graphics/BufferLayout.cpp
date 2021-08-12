#include <redoom/graphics/BufferLayout.hh>

#include <numeric>

namespace redoom::graphics
{
namespace detail
{
static unsigned int shaderTypeComponentCount(ShaderDataType type)
{
  switch (type) {
    case ShaderDataType::Float: return 1;
    case ShaderDataType::Float2: return 2;
    case ShaderDataType::Float3: return 3;
    case ShaderDataType::Float4: return 4;
    case ShaderDataType::Mat3: return 3 * 3;
    case ShaderDataType::Mat4: return 4 * 4;
    case ShaderDataType::Int: return 1;
    case ShaderDataType::Int2: return 2;
    case ShaderDataType::Int3: return 3;
    case ShaderDataType::Int4: return 4;
    case ShaderDataType::Bool: return 1;
  }
  static_assert("Unknown ShaderDataType" != nullptr);
  return 0;
}

static unsigned int shaderDataTypeSize(ShaderDataType type)
{
  switch (type) {
    case ShaderDataType::Float:
    case ShaderDataType::Float2:
    case ShaderDataType::Float3:
    case ShaderDataType::Float4:
    case ShaderDataType::Mat3:
    case ShaderDataType::Mat4:
      return sizeof(float) * shaderTypeComponentCount(type);
    case ShaderDataType::Int:
    case ShaderDataType::Int2:
    case ShaderDataType::Int3:
    case ShaderDataType::Int4:
      return sizeof(int) * shaderTypeComponentCount(type);
    case ShaderDataType::Bool:
      return sizeof(bool) * shaderTypeComponentCount(type);
  }
  static_assert("Unknown ShaderDataType" != nullptr);
  return 0;
}

static GLenum shaderDataTypeToGLenum(ShaderDataType type)
{
  switch (type) {
    case ShaderDataType::Float:
    case ShaderDataType::Float2:
    case ShaderDataType::Float3:
    case ShaderDataType::Float4:
    case ShaderDataType::Mat3:
    case ShaderDataType::Mat4: return GL_FLOAT;
    case ShaderDataType::Int:
    case ShaderDataType::Int2:
    case ShaderDataType::Int3:
    case ShaderDataType::Int4: return GL_INT;
    case ShaderDataType::Bool: return GL_BOOL;
  }
  static_assert("Unknown ShaderDataType" != nullptr);
  return 0;
}
} // namespace detail

BufferElement::BufferElement(
    ShaderDataType ptype, std::string pname, bool pnormalized) noexcept
  : type{ptype}
  , gl_type{detail::shaderDataTypeToGLenum(ptype)}
  , name{std::move(pname)}
  , normalized{pnormalized}
  , size{detail::shaderDataTypeSize(ptype)}
  , components_count{detail::shaderTypeComponentCount(ptype)}
{
}

BufferLayout::BufferLayout(
    std::initializer_list<BufferElement> elements) noexcept
  : std::vector<BufferElement>{elements}
  , stride{
        std::accumulate(elements.begin(),
            elements.end(),
            0u,
            [](auto acc, auto const& element) { return acc + element.size; }),
    }
{
}
} // namespace redoom::graphics
