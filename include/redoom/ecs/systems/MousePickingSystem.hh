#pragma once

#include <redoom/ecs/System.hh>
#include <redoom/graphics/FrameBuffer.hh>

namespace redoom::ecs::systems
{
struct MousePickingSystem : public MonothreadedSystem<MousePickingSystem> {
  void onAttach() noexcept override;

  void update(UpdateContext& context) noexcept override;
};
} // namespace redoom::ecs::systems
