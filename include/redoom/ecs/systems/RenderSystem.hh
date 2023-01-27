#pragma once

#include <redoom/ecs/System.hh>

namespace redoom::ecs::systems
{
struct RenderSystem : public MonothreadedSystem<RenderSystem> {
  void beforeUpdate(UpdateContext& context) noexcept override;
  void update(UpdateContext& context) noexcept override;
  void afterUpdate(UpdateContext& context) noexcept override;
};
} // namespace redoom::ecs::systems
