#pragma once

#include <redoom/ecs/System.hh>

namespace redoom::ecs::systems
{
struct BehaviourSystem : public MonothreadedSystem<BehaviourSystem> {
  void update(UpdateContext& context) noexcept override;
};
} // namespace redoom::ecs::systems
