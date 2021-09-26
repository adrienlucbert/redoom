#include <redoom/ecs/components/BehaviourComponent.hh>

#include <redoom/Scene.hh>
#include <redoom/ecs/behaviours/CameraBehaviour.hh>

namespace redoom::ecs::components
{
BehaviourComponent::BehaviourComponent(
    std::unique_ptr<Behaviour> pbehaviour) noexcept
  : behaviour{std::move(pbehaviour)}
{
}

std::string const& BehaviourComponent::getType() const noexcept
{
  static auto const type = std::string{"BehaviourComponent"};
  return type;
}

void BehaviourComponent::Serializer::serialize(
    YAML::Emitter& out, ecs::ComponentBase const* component) const noexcept
{
  auto const* bc = dynamic_cast<BehaviourComponent const*>(component);
  out << bc->behaviour->getType();
}
[[nodiscard]] Expected<> BehaviourComponent::Serializer::deserialize(
    YAML::Node const& /*node*/, Scene& scene, Entity entity) const noexcept
{
  scene.getRegistry().attachComponent<behaviours::CameraBehaviour>(entity);
  return {};
}
} // namespace redoom::ecs::components
