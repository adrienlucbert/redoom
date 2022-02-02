#pragma once

#include <redoom/Scene.hh>
#include <redoom/ecs/Component.hh>
#include <redoom/ecs/components/BodyComponent.hh>
#include <redoom/graphics/Model.hh>
#include <redoom/serializer/ComponentSerializer.hh>

namespace redoom::ecs::components
{
using redoom::graphics::Model;

struct ModelComponent : public Component<ModelComponent> {
  [[nodiscard]] std::string const& getType() const noexcept override
  {
    static auto const type = std::string{"ModelComponent"};
    return type;
  }

  explicit ModelComponent(graphics::Model pmodel)
    : model{std::move(pmodel)}
  {
  }

  graphics::Model model;

  struct Serializer : public ComponentSerializer {
    void serialize(
        YAML::Emitter& out, ecs::ComponentBase const* component) const override
    {
      auto const* mc = dynamic_cast<ModelComponent const*>(component);
      auto path_opt = mc->model.getPath();
      if (path_opt)
        out << YAML::Key << "path" << YAML::Value << *path_opt;
      auto options_opt = mc->model.getImporterOptions();
      if (options_opt) {
        out << YAML::Key << "flip_uvs" << YAML::Value << options_opt->flip_uvs;
      }
    }

    [[nodiscard]] Expected<> deserialize(
        YAML::Node const& node, Scene& scene, Entity entity) const override
    {
      auto path = node["path"].as<std::string>();
      auto importer_options = graphics::ModelImporterOptions{};
      if (node["flip_uvs"])
        importer_options.flip_uvs = node["flip_uvs"].as<bool>();
      auto exp = Model::fromFile(std::move(path), importer_options);
      RETURN_IF_UNEXPECTED(exp);
      // TODO(alucbert): remove this (temporary body creation helper)
      // scene.getRegistry().attachComponent<BodyComponent>(
      //     entity, BodyComponent::fromModel(scene.getWorld(), {}, *exp));
      scene.getRegistry().attachComponent<ModelComponent>(
          entity, std::move(*exp));
      return {};
    }
  };
};
} // namespace redoom::ecs::components
