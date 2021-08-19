#pragma once

#include <redoom/ecs/System.hh>

namespace mock
{
struct TestSystem1 final : public redoom::ecs::System<TestSystem1> {
  void update(redoom::ecs::UpdateContext& /*context*/) noexcept override;

  static bool isUpdated; // NOLINT
};

struct TestSystem2 final : public redoom::ecs::System<TestSystem2> {
  void update(redoom::ecs::UpdateContext& /*context*/) noexcept override;

  static bool isUpdated; // NOLINT
};
} // namespace mock
