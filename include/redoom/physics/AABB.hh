#pragma once

#include <glm/gtc/type_ptr.hpp>

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

  [[nodiscard]] glm::vec3 getCenter() const noexcept
  {
    return 0.5f * (this->lower_bounds + this->upper_bounds);
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
};
} // namespace redoom::physics
