#pragma once

#include <catch2/catch.hpp>

#include <fmt/format.h>
#include <glm/gtc/type_ptr.hpp>

namespace Catch
{
template <>
struct StringMaker<glm::vec2> {
  static std::string convert(glm::vec2 const& vec)
  {
    return fmt::format("{: .2f} {: .2f}", vec.x, vec.y);
  }
};

template <>
struct StringMaker<glm::vec3> {
  static std::string convert(glm::vec3 const& vec)
  {
    return fmt::format("{: .2f} {: .2f} {: .2f}", vec.x, vec.y, vec.z);
  }
};
} // namespace Catch
