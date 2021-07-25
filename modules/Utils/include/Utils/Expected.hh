#pragma once

#include <string>
#include <utility>

#include <fmt/format.h>
#include <tl/expected.hpp>

#include <Utils/Error.hh>

namespace redoom
{
template <typename T = void>
using Expected = tl::expected<T, errors::Error>;

template <typename S, typename... Args>
tl::unexpected<std::string> make_formatted_unexpected(
    S const& format, Args&&... args) noexcept
{
  return tl::make_unexpected(fmt::format(format, std::forward<Args>(args)...));
}
} // namespace redoom

// NOLINTNEXTLINE(cppcoreguidelines-macro-usage)
#define RETURN_UNEXPECTED(exp) \
  return tl::make_unexpected(std::move((exp).error()))

// NOLINTNEXTLINE(cppcoreguidelines-macro-usage)
#define RETURN_IF_UNEXPECTED(exp) \
  do {                            \
    if (!(exp))                   \
      RETURN_UNEXPECTED(exp);     \
  } while (0)
