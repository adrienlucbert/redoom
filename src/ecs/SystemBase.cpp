#include <redoom/ecs/SystemBase.hh>

namespace redoom::ecs
{
SystemBase::SystemBase(bool pis_multithreaded) noexcept
  : is_multithreaded{pis_multithreaded}
{
}

void SystemBase::update(UpdateContext& /*unused*/) noexcept
{
}
} // namespace redoom::ecs
