#pragma once

#include <atomic>

namespace redoom::Utils
{
using type_id = unsigned int;

namespace detail
{
[[nodiscard]] type_id getNextTypeId() noexcept;
} // namespace detail

template <typename T>
class UniqueTypeId
{
public:
  UniqueTypeId(UniqueTypeId const& b) noexcept = default;
  UniqueTypeId(UniqueTypeId&& b) noexcept = default;
  virtual ~UniqueTypeId() = default;

  UniqueTypeId& operator=(UniqueTypeId const& rhs) noexcept = default;
  UniqueTypeId& operator=(UniqueTypeId&& rhs) noexcept = default;

  [[nodiscard]] static type_id getTypeId() noexcept
  {
    static auto type_id = detail::getNextTypeId();
    return type_id;
  }

protected:
  UniqueTypeId() = default;
};
} // namespace redoom::Utils
