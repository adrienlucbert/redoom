#pragma once

#include <redoom/ecs/Behaviour.hh>

#include <glm/gtc/type_ptr.hpp>

#include <redoom/ecs/components/TransformComponent.hh>

struct DonutBehaviour : public redoom::ecs::Behaviour {
  void onUpdate(redoom::ecs::Entity entity,
      redoom::ecs::UpdateContext& context) noexcept override
  {
    auto transform_opt =
        context.component_manager
            .get<redoom::ecs::components::TransformComponent>(entity);
    auto& transform = *transform_opt;
    transform.angle += static_cast<float>(context.elapsed_time * 5);
  }
};
