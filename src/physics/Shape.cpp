#include <redoom/physics/Shape.hh>

namespace redoom::physics
{
Shape::Shape(std::string_view type) noexcept
  : type_{type}
{
}

std::string_view Shape::getType() const noexcept
{
  return this->type_;
}
} // namespace redoom::physics
