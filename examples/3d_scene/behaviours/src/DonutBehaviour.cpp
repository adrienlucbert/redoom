#include <behaviours/extern.hh>

#include <GL/glew.h>
#include <GLFW/glfw3.h>
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
    if (!transform_opt)
      assert("No transform component associated" == nullptr);
    try {
      auto& transform = transform_opt.value();
      transform.angle +=
          static_cast<float>(context.elapsed_time * this->rotation_speed);
    } catch (tl::bad_optional_access const&) {
      // NOTE: this should never happen, but it makes clang-tidy happy
    }
  }
};

std::unique_ptr<Behaviour> make() noexcept
{
  return std::make_unique<DonutBehaviour>();
}
