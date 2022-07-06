#include <redoom/graphics/UniformBase.hh>

#include <redoom/graphics/Program.hh>

namespace redoom::graphics
{
UniformBase::UniformBase(std::any data) noexcept
  : data_{std::move(data)}
{
}
} // namespace redoom::graphics
