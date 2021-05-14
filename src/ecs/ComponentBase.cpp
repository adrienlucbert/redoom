#include <redoom/ecs/ComponentBase.hh>

#include <atomic>

namespace redoom::ecs
{
unsigned int ComponentBase::getNextTypeId() noexcept
{
  static std::atomic<unsigned int> id = 0;
  return id++;
}
}
