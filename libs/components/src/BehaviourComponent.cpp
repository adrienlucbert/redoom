#include <components/extern.hh>

#include <memory>

#include <Utils/Factory.hh>
#include <redoom/ecs/Behaviour.hh>
#include <redoom/ecs/components/BehaviourComponent.hh>

using redoom::ecs::Behaviour;
using redoom::ecs::components::BehaviourComponent;

// NOLINTNEXTLINE
static auto factory = redoom::Utils::Factory<Behaviour,
    void (*)(YAML::Emitter&, BehaviourComponent const*),
    redoom::Expected<std::unique_ptr<Behaviour>> (*)(
        YAML::Node const&, redoom::Scene&, redoom::ecs::Entity)>{};

using redoom::ecs::components::BehaviourComponent;

redoom::Utils::type_id getTypeId() noexcept
{
  return BehaviourComponent::getTypeId();
}

void serialize(YAML::Emitter& out, redoom::ecs::ComponentBase const* component)
{
  auto const* bc = dynamic_cast<BehaviourComponent const*>(component);
  out << bc->behaviour_->getType();
  factory.serialize(
      fmt::format("lib/lib{}.so", bc->behaviour_->getType()), out, bc);
}

redoom::Expected<> deserialize(
    YAML::Node const& node, redoom::Scene& scene, redoom::ecs::Entity entity)
{
  auto const& type = node.as<std::string>();
  auto behaviour_exp = factory.deserialize(
      fmt::format("lib/lib{}.so", type), node, scene, entity);
  RETURN_IF_UNEXPECTED(behaviour_exp);
  scene.getRegistry().attachComponent<BehaviourComponent>(
      entity, std::move(*behaviour_exp));
  return {};
}
