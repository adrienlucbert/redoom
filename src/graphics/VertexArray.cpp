#include <redoom/graphics/VertexArray.hh>

namespace redoom::graphics
{
VertexArray::VertexArray() noexcept
{
  glGenVertexArrays(1, &this->id);
}

VertexArray::~VertexArray() noexcept
{
  glDeleteVertexArrays(1, &this->id);
}

void VertexArray::bind() const noexcept
{
  glBindVertexArray(this->id);
}

void VertexArray::unbind() noexcept
{
  glBindVertexArray(0);
}
} // namespace redoom::graphics
