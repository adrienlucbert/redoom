#pragma once

#include <algorithm>
#include <iostream>
#include <ostream>

#include <fmt/format.h>
#include <glm/gtc/type_ptr.hpp>

namespace redoom::physics
{
struct AABB;
} // namespace redoom::physics

std::ostream& operator<<(
    std::ostream& out, redoom::physics::AABB const& aabb) noexcept;

namespace redoom::physics
{
struct AABB {
  glm::vec3 lower_bounds{0.0f, 0.0f, 0.0f};
  glm::vec3 upper_bounds{0.0f, 0.0f, 0.0f};

  template <typename T>
  [[nodiscard]] AABB operator*(T const& rhs) const noexcept
  {
    return AABB{this->lower_bounds * rhs, this->upper_bounds * rhs};
  }
  template <typename T>
  AABB& operator*=(T const& rhs) noexcept
  {
    *this = *this * rhs;
    return *this;
  }
  template <typename T>
  [[nodiscard]] AABB operator/(T const& rhs) const noexcept
  {
    return AABB{this->lower_bounds / rhs, this->upper_bounds / rhs};
  }
  template <typename T>
  AABB& operator/=(T const& rhs) noexcept
  {
    *this = *this / rhs;
    return *this;
  }
  template <typename T>
  [[nodiscard]] AABB operator+(T const& rhs) const noexcept
  {
    return AABB{this->lower_bounds + rhs, this->upper_bounds + rhs};
  }
  template <typename T>
  AABB& operator+=(T const& rhs) noexcept
  {
    *this = *this + rhs;
    return *this;
  }
  template <typename T>
  [[nodiscard]] AABB operator-(T const& rhs) const noexcept
  {
    return AABB{this->lower_bounds - rhs, this->upper_bounds - rhs};
  }
  template <typename T>
  AABB& operator-=(T const& rhs) noexcept
  {
    *this = *this - rhs;
    return *this;
  }
  bool operator==(AABB const& rhs) const noexcept
  {
    auto result = true;
    result &= std::fabs(this->lower_bounds.x - rhs.lower_bounds.x)
           <= std::numeric_limits<float>::epsilon();
    result &= std::fabs(this->lower_bounds.z - rhs.lower_bounds.z)
           <= std::numeric_limits<float>::epsilon();
    result &= std::fabs(this->lower_bounds.y - rhs.lower_bounds.y)
           <= std::numeric_limits<float>::epsilon();
    result &= std::fabs(this->upper_bounds.x - rhs.upper_bounds.x)
           <= std::numeric_limits<float>::epsilon();
    result &= std::fabs(this->upper_bounds.z - rhs.upper_bounds.z)
           <= std::numeric_limits<float>::epsilon();
    result &= std::fabs(this->upper_bounds.y - rhs.upper_bounds.y)
           <= std::numeric_limits<float>::epsilon();
    return result;
  }
  bool operator!=(AABB const& rhs) const noexcept
  {
    return !(*this == rhs);
  }

  [[nodiscard]] glm::vec3 getCenter() const noexcept
  {
    return 0.5f * (this->lower_bounds + this->upper_bounds);
  }

  [[nodiscard]] glm::vec3 getSize() const noexcept
  {
    return this->upper_bounds - this->lower_bounds;
  }

  [[nodiscard]] AABB combine(AABB const& rhs) const noexcept
  {
    return AABB{{std::min(this->lower_bounds.x, rhs.lower_bounds.x),
                    std::min(this->lower_bounds.y, rhs.lower_bounds.y),
                    std::min(this->lower_bounds.z, rhs.lower_bounds.z)},
        {std::max(this->upper_bounds.x, rhs.upper_bounds.x),
            std::max(this->upper_bounds.y, rhs.upper_bounds.y),
            std::max(this->upper_bounds.z, rhs.upper_bounds.z)}};
  }

  [[nodiscard]] bool contains(AABB const& rhs) const noexcept
  {
    auto result = true;
    result &= this->lower_bounds.x <= rhs.lower_bounds.x;
    result &= this->lower_bounds.y <= rhs.lower_bounds.y;
    result &= this->lower_bounds.z <= rhs.lower_bounds.z;
    result &= rhs.upper_bounds.x <= this->upper_bounds.x;
    result &= rhs.upper_bounds.y <= this->upper_bounds.y;
    result &= rhs.upper_bounds.z <= this->upper_bounds.z;
    return result;
  }

  [[nodiscard]] bool contains(glm::vec3 const& point) const noexcept
  {
    auto result = true;
    result &= this->lower_bounds.x <= point.x;
    result &= this->lower_bounds.y <= point.y;
    result &= this->lower_bounds.z <= point.z;
    result &= point.x <= this->upper_bounds.x;
    result &= point.y <= this->upper_bounds.y;
    result &= point.z <= this->upper_bounds.z;
    return result;
  }

  [[nodiscard]] bool intersects(AABB const& rhs) const noexcept
  {
    auto result = true;
    result &= this->lower_bounds.x <= rhs.upper_bounds.x
           && this->upper_bounds.x >= rhs.lower_bounds.x;
    result &= this->lower_bounds.y <= rhs.upper_bounds.y
           && this->upper_bounds.y >= rhs.lower_bounds.y;
    result &= this->lower_bounds.z <= rhs.upper_bounds.z
           && this->upper_bounds.z >= rhs.lower_bounds.z;
    return result;
  }

  // [[nodiscard]] bool intersects(glm::vec3 const& vector) const noexcept
  // {
  //   // TODO(alucbert): intersection with a ray
  // }
};
} // namespace redoom::physics

inline std::ostream& operator<<(
    std::ostream& out, redoom::physics::AABB const& aabb) noexcept
{
  out << fmt::format("/{: .2f} {: .2f} {: .2f}\\\n\\{: .2f} {: .2f} {: .2f}/",
      aabb.lower_bounds.x,
      aabb.lower_bounds.y,
      aabb.lower_bounds.z,
      aabb.upper_bounds.x,
      aabb.upper_bounds.y,
      aabb.upper_bounds.z)
      << '\n';
  return out;
}
