#pragma once

namespace redoom
{
enum class Platform { Unknown, Linux };

constexpr Platform getPlatform() noexcept;
}; // namespace redoom
