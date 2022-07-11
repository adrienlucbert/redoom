#include <components/extern.hh>

#include <stdexcept>

#include <Utils/Factory.hh>
#include <redoom/ecs/components/MeshComponent.hh>
#include <redoom/graphics/Mesh.hh>

using redoom::ecs::components::MeshComponent;

redoom::Utils::type_id getTypeId() noexcept
{
  return MeshComponent::getTypeId();
}

// NOLINTNEXTLINE
static auto factory = redoom::Utils::Factory<redoom::graphics::Mesh,
    void (*)(YAML::Emitter&, std::shared_ptr<redoom::graphics::Mesh> const&),
    redoom::Expected<std::shared_ptr<redoom::graphics::Mesh>> (*)(
        YAML::Node const&)>{};

void serialize(YAML::Emitter& out, redoom::ecs::ComponentBase const* component)
{
  auto const* mc = dynamic_cast<MeshComponent const*>(component);
  auto const& type = mc->mesh_->getType();
  out << YAML::Key << "type" << YAML::Value << type;
  factory.serialize(fmt::format("lib/lib{}.so", type), out, mc->mesh_);
}

redoom::Expected<> deserialize(
    YAML::Node const& node, redoom::Scene& scene, redoom::ecs::Entity entity)
{
  auto const& type = node["type"].as<std::string>();
  auto mesh_exp = factory.deserialize(fmt::format("lib/lib{}.so", type), node);
  RETURN_IF_UNEXPECTED(mesh_exp);
  scene.getRegistry().attachComponent<MeshComponent>(
      entity, std::move(*mesh_exp));
  return {};
}
