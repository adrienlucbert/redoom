#include <Utils/UniqueTypeId.hh>

namespace redoom::Utils
{
type_id detail::getNextTypeId() noexcept
{
  static std::atomic<type_id> id = 0;
  return id++;
}
} // namespace redoom::Utils
