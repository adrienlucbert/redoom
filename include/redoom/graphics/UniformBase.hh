#pragma once

#include <any>
#include <string_view>

namespace redoom::graphics
{
class Program;

class UniformBase
{
public:
  explicit UniformBase(std::any data) noexcept;
  virtual ~UniformBase() noexcept = default;

  virtual void set(
      Program const& program, std::string_view name) const noexcept = 0;

protected:
  std::any data_;
};

} // namespace redoom::graphics
