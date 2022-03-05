#include <redoom/graphics/VertexArray.hh>

#include <utility>

namespace redoom::graphics
{
VertexArray::VertexArray() noexcept
  : id_{}
{
  glGenVertexArrays(1, &this->id_);
}

VertexArray::VertexArray(VertexArray&& b) noexcept
  : id_{b.id_}
{
  b.id_ = 0;
}

VertexArray::~VertexArray() noexcept
{
  if (this->id_ != 0)
    glDeleteVertexArrays(1, &this->id_);
}

VertexArray& VertexArray::operator=(VertexArray&& rhs) noexcept
{
  if (this != &rhs)
    std::swap(this->id_, rhs.id_);
  return *this;
}

void VertexArray::bind() const noexcept
{
  glBindVertexArray(this->id_);
}

void VertexArray::unbind() const noexcept // NOLINT
{
  glBindVertexArray(0);
}
} // namespace redoom::graphics
