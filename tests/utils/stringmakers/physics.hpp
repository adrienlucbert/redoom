#pragma once

#include <catch2/catch.hpp>

#include <redoom/physics/AABB.hh>

namespace Catch
{
template <>
struct StringMaker<redoom::physics::AABB> {
  static std::string convert(redoom::physics::AABB const& aabb)
  {
    return static_cast<std::string>(aabb);
  }
};
} // namespace Catch
