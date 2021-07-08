#include <redoom/ecs/SystemBase.hh>

namespace redoom::ecs
{
void SystemBase::update(long /*unused*/) noexcept
{
}

void SystemBase::update(long /*unused*/, ComponentManager& /*unused*/) noexcept
{
}
} // namespace redoom::ecs
