#pragma once

#include <string_view>

#include <redoom/graphics/Program.hh>
#include <redoom/graphics/UniformBase.hh>
#include <redoom/graphics/UniformTypeSetter.hh>

namespace redoom::graphics
{
template <typename T>
class UnboundUniform : public UniformBase
{
public:
  explicit UnboundUniform(std::any data) noexcept
    : UniformBase(data)
  {
  }

  void set(
      Program const& program, std::string_view name) const noexcept override
  {
    auto location = glGetUniformLocation(program.getId(), name.data());
    UniformTypeSetter<T>::set(location, this->data_);
  }
};
} // namespace redoom::graphics
