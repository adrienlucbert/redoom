#pragma once

#include <redoom/ecs/System.hh>

namespace redoom::ecs::systems
{
struct PhysicsWorldSystem : public MultithreadedSystem<PhysicsWorldSystem> {
  void update(UpdateContext& context) noexcept override;
};
} // namespace redoom::ecs::systems
