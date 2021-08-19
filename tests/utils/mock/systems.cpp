#include <mock/systems.hh>

namespace mock
{
bool TestSystem1::isUpdated = false; // NOLINT
void TestSystem1::update(redoom::ecs::UpdateContext& /*context*/) noexcept
{
  mock::TestSystem1::isUpdated = true;
}

bool TestSystem2::isUpdated = false; // NOLINT
void TestSystem2::update(redoom::ecs::UpdateContext& /*context*/) noexcept
{
  mock::TestSystem2::isUpdated = true;
}
} // namespace mock
