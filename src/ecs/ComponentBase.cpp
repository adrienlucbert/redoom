#include <redoom/ecs/ComponentBase.hh>

namespace redoom::ecs
{
void ComponentBase::onInit(Entity /*entity*/, Context& /*context*/) noexcept
{
}

void ComponentBase::onDestroy(Entity /*entity*/, Context& /*context*/) noexcept
{
}
} // namespace redoom::ecs
