#pragma once

#include <redoom/ecs/System.hh>

namespace redoom::ecs::systems
{
struct MousePickingSystem : public MonothreadedSystem<MousePickingSystem> {
  void update(UpdateContext& context) noexcept override;
};
} // namespace redoom::ecs::systems
