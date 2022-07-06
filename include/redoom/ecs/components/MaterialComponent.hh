#pragma once

#include <memory>

#include <redoom/Scene.hh>
#include <redoom/ecs/Component.hh>
#include <redoom/graphics/Material.hh>
#include <redoom/serializer/ComponentSerializer.hh>
#include <redoom/serializer/common.hh>

namespace redoom::ecs::components
{
struct MaterialComponent : public Component<MaterialComponent> {
  [[nodiscard]] std::string const& getType() const noexcept override
  {
    static auto const type = std::string{"MaterialComponent"};
    return type;
  }

  explicit MaterialComponent(graphics::Material pmaterial)
    : material{std::move(pmaterial)}
  {
  }

  graphics::Material material;

  struct Serializer : public ComponentSerializer {
    void serialize(
        YAML::Emitter& out, ecs::ComponentBase const* component) const override
    {
      auto const* mc = dynamic_cast<MaterialComponent const*>(component);
      out << YAML::Key << "shader" << YAML::Value << mc->material.shader;
      out << YAML::Key << "gloss" << YAML::Value << mc->material.gloss;
      out << YAML::Key << "smoothness" << YAML::Value
          << mc->material.smoothness;
      out << YAML::Key << "metallicness" << YAML::Value
          << mc->material.metallicness;
    }

    [[nodiscard]] Expected<> deserialize(
        YAML::Node const& node, Scene& scene, Entity entity) const override
    {
      auto shader = std::string{"lit"};
      if (node["shader"])
        shader = node["shader"].as<std::string>();
      auto gloss = 6.0f;
      if (node["gloss"])
        gloss = node["gloss"].as<float>();
      auto smoothness = 1.0f;
      if (node["smoothness"])
        smoothness = node["smoothness"].as<float>();
      auto metallicness = 1.0f;
      if (node["metallicness"])
        metallicness = node["metallicness"].as<float>();
      scene.getRegistry().attachComponent<MaterialComponent>(entity,
          graphics::Material{
              std::move(shader), gloss, smoothness, metallicness});
      return {};
    }
  };
};
} // namespace redoom::ecs::components
