#include <redoom/ecs/SystemBase.hh>

namespace redoom::ecs
{
SystemBase::SystemBase(bool pis_multithreaded) noexcept
  : is_multithreaded_{pis_multithreaded}
{
}

void SystemBase::onAttach() noexcept
{
}

void SystemBase::beforeUpdate(UpdateContext& /*unused*/) noexcept
{
}

void SystemBase::update(UpdateContext& /*unused*/) noexcept
{
}

void SystemBase::afterUpdate(UpdateContext& /*unused*/) noexcept
{
}

void SystemBase::onDetach() noexcept
{
}

bool SystemBase::isMultithreaded() const noexcept
{
  return this->is_multithreaded_;
}
} // namespace redoom::ecs
