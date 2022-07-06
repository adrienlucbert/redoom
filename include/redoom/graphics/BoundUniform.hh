#pragma once

#include <any>
#include <string_view>
#include <type_traits>
#include <utility>

#include <redoom/graphics/UniformBase.hh>
#include <redoom/graphics/UniformTypeSetter.hh>

namespace redoom::graphics
{
class Program;

template <typename T>
class BoundUniform : public UniformBase
{
public:
  explicit BoundUniform(int location, std::any data) noexcept
    : UniformBase(data)
    , location_{location}
  {
  }

  void set(Program const& /*program*/,
      std::string_view /*name*/) const noexcept override
  {
    UniformTypeSetter<T>::set(this->location_, this->data_);
  }

protected:
  int location_;
};
} // namespace redoom::graphics
