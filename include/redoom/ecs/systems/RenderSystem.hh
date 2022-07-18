#pragma once

#include <redoom/ecs/System.hh>

namespace redoom::ecs::systems
{
struct RenderSystem : public MonothreadedSystem<RenderSystem> {
  void update(UpdateContext& context) noexcept override;
};
} // namespace redoom::ecs::systems
