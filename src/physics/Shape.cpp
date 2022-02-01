#include <redoom/physics/Shape.hh>

namespace redoom::physics
{
Shape::Shape(std::string_view ptype) noexcept
  : type{ptype}
{
}

std::string_view Shape::getType() const noexcept
{
  return this->type;
}
} // namespace redoom::physics
