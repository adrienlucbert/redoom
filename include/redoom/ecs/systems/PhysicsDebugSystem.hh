#pragma once

#include <redoom/ecs/System.hh>

namespace redoom::ecs::systems
{
struct PhysicsDebugSystem : public MonothreadedSystem<PhysicsDebugSystem> {
  void update(UpdateContext& context) noexcept override;
};
} // namespace redoom::ecs::systems
