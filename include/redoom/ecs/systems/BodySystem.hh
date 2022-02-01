#pragma once

#include <redoom/ecs/System.hh>

namespace redoom::ecs::systems
{
struct BodySystem : public MonothreadedSystem<BodySystem> {
  void update(UpdateContext& context) noexcept override;
};
} // namespace redoom::ecs::systems
