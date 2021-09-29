#include <redoom/ecs/components/BehaviourComponent.hh>

#include <Utils/Expected.hh>
#include <Utils/Factory.hh>
#include <redoom/Scene.hh>
#include <redoom/ecs/Behaviour.hh>

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
    YAML::Node const& node, Scene& scene, Entity entity) const noexcept
{
  static auto factories =
      std::unordered_map<std::string, Utils::Factory<Behaviour>>{};
  auto const& type = node.as<std::string>();
  auto exp =
      [&]() -> Expected<std::reference_wrapper<Utils::Factory<Behaviour>>> {
    auto factory_it = factories.find(type);
    if (factory_it == factories.end()) {
      auto const path = fmt::format("lib/lib{}.so", type);
      auto factory_exp = Utils::Factory<Behaviour>::fromDL(path, "make");
      RETURN_IF_UNEXPECTED(factory_exp);
      auto [it, success] = factories.emplace(type, std::move(*factory_exp));
      if (!success)
        return make_formatted_unexpected("Dynamic library {} not found", type);
      return it->second;
    } else {
      return factory_it->second;
    }
  }();
  RETURN_IF_UNEXPECTED(exp);
  auto& factory = *exp;
  auto component = factory.get().make();
  scene.getRegistry().attachComponent<BehaviourComponent>(
      entity, std::move(component));
  return {};
}
} // namespace redoom::ecs::components
