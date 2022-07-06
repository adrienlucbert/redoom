#pragma once

#include <redoom/ecs/System.hh>

namespace redoom::ecs::systems
{
struct LightSystem : public MonothreadedSystem<LightSystem> {
  void update(UpdateContext& context) noexcept override;
};
} // namespace redoom::ecs::systems
