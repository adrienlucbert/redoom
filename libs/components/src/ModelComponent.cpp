#include <components/extern.hh>

#include <stdexcept>

#include <redoom/ecs/components/ModelComponent.hh>
#include <redoom/graphics/Model.hh>

using redoom::ecs::components::ModelComponent;
using redoom::graphics::Model;

redoom::Utils::type_id getTypeId() noexcept
{
  return ModelComponent::getTypeId();
}

void serialize(YAML::Emitter& out, redoom::ecs::ComponentBase const* component)
{
  auto const* mc = dynamic_cast<ModelComponent const*>(component);
  auto path_opt = mc->model_.getPath();
  if (path_opt)
    out << YAML::Key << "path" << YAML::Value << *path_opt;
  auto options_opt = mc->model_.getImporterOptions();
  if (options_opt) {
    out << YAML::Key << "flip_uvs" << YAML::Value << options_opt->flip_uvs;
  }
}

redoom::Expected<> deserialize(
    YAML::Node const& node, redoom::Scene& scene, redoom::ecs::Entity entity)
{
  auto path = node["path"].as<std::string>();
  auto importer_options = redoom::graphics::ModelImporterOptions{};
  if (node["flip_uvs"])
    importer_options.flip_uvs = node["flip_uvs"].as<bool>();
  auto exp = Model::fromFile(std::move(path), importer_options);
  RETURN_IF_UNEXPECTED(exp);
  scene.getRegistry().attachComponent<ModelComponent>(entity, std::move(*exp));
  return {};
}
