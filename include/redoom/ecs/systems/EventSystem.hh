#pragma once

#include <redoom/ecs/System.hh>

namespace redoom::ecs::systems
{
struct EventSystem : public MonothreadedSystem<EventSystem> {
  void update(UpdateContext& context) noexcept override;
};
} // namespace redoom::ecs::systems
