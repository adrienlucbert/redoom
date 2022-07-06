#pragma once

#include <any>

namespace redoom::graphics
{
template <typename T>
struct UniformTypeSetter {
  static void set(int location, std::any const& data) noexcept = 0;
};
} // namespace redoom::graphics
