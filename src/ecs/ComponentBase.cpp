#include <redoom/ecs/ComponentBase.hh>

#include <atomic>

namespace redoom::ecs
{
ComponentBase::~ComponentBase() noexcept = default;

unsigned int ComponentBase::getNextTypeId() noexcept
{
  static std::atomic<unsigned int> id = 0;
  return id++;
}
} // namespace redoom::ecs
