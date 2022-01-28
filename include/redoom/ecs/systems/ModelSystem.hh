#pragma once

#include <redoom/ecs/System.hh>

namespace redoom::ecs::systems
{
struct ModelSystem : public MonothreadedSystem<ModelSystem> {
  void update(UpdateContext& context) noexcept override;
};
} // namespace redoom::ecs::systems
