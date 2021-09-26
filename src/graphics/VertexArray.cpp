#include <redoom/graphics/VertexArray.hh>

#include <utility>

namespace redoom::graphics
{
VertexArray::VertexArray() noexcept
  : id{}
{
  glGenVertexArrays(1, &this->id);
}

VertexArray::VertexArray(VertexArray&& b) noexcept
  : id{b.id}
{
  b.id = 0;
}

VertexArray::~VertexArray() noexcept
{
  if (this->id != 0)
    glDeleteVertexArrays(1, &this->id);
}

VertexArray& VertexArray::operator=(VertexArray&& rhs) noexcept
{
  if (this != &rhs)
    std::swap(this->id, rhs.id);
  return *this;
}

void VertexArray::bind() const noexcept
{
  glBindVertexArray(this->id);
}

void VertexArray::unbind() const noexcept // NOLINT
{
  glBindVertexArray(0);
}
} // namespace redoom::graphics
