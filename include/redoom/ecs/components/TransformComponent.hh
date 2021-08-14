#include <redoom/ecs/Component.hh>

#include <glm/vec3.hpp>

namespace redoom::ecs::components
{
struct Transform : public Component<Transform> {
  glm::vec3 position{0.0f, 0.0f, 0.0f};
  glm::vec3 rotation{0.0f, 0.0f, 0.0f};
  glm::vec3 scale{1.0f, 1.0f, 1.0f};
};
} // namespace redoom::ecs::components
