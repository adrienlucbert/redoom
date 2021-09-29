#include <behaviours/extern.hh>

#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <fmt/format.h>
#include <glm/gtc/type_ptr.hpp>

#include <redoom/ecs/Behaviour.hh>
#include <redoom/ecs/components/TransformComponent.hh>

using redoom::ecs::Behaviour;
using redoom::ecs::Entity;
using redoom::ecs::UpdateContext;

struct DonutBehaviour : public Behaviour {
  double rotation_speed{5.0};

  [[nodiscard]] std::string const& getType() const noexcept override
  {
    static auto const type = std::string{"DonutBehaviour"};
    return type;
  }

  void onUpdate(Entity entity, UpdateContext& context) noexcept override
  {
    auto transform_opt =
        context.component_manager
            .get<redoom::ecs::components::TransformComponent>(entity);
    auto& transform = *transform_opt;
    transform.angle +=
        static_cast<float>(context.elapsed_time * this->rotation_speed);
  }
};

std::unique_ptr<Behaviour> make() noexcept
{
  return std::make_unique<DonutBehaviour>();
}
