#include <redoom/platform/Platform.hh>

namespace redoom
{
constexpr Platform getPlatform() noexcept
{
#if defined(__linux__)
  return Platform::Linux;
#else
  return Platform::Unknown;
#endif
}
} // namespace redoom
