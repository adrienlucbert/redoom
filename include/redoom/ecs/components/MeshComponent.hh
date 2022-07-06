#pragma once

#include <memory>

#include <redoom/Scene.hh>
#include <redoom/ecs/Component.hh>
#include <redoom/graphics/Mesh.hh>
#include <redoom/graphics/mesh/Sphere.hh>
#include <redoom/serializer/ComponentSerializer.hh>
#include <redoom/serializer/common.hh>

namespace redoom::ecs::components
{
using redoom::graphics::mesh::Sphere;

struct MeshComponent : public Component<MeshComponent> {
  [[nodiscard]] std::string const& getType() const noexcept override
  {
    static auto const type = std::string{"MeshComponent"};
    return type;
  }

  explicit MeshComponent(std::shared_ptr<graphics::Mesh> mesh)
    : mesh_{std::move(mesh)}
  {
  }

  std::shared_ptr<graphics::Mesh> mesh_;

  struct Serializer : public ComponentSerializer {
    void serialize(YAML::Emitter& /*out*/,
        ecs::ComponentBase const* /*component*/) const override
    {
      // TODO(alucbert): serialize meshes
      // auto const* mc = dynamic_cast<MeshComponent const*>(component);
    }

    [[nodiscard]] Expected<> deserialize(
        YAML::Node const& node, Scene& scene, Entity entity) const override
    {
      // TODO(alucbert): deserialize meshes
      auto color = glm::vec3{1.0f, 0.4f, 0.6f};
      if (node["color"])
        color = node["color"].as<glm::vec3>();
      auto mesh =
          std::shared_ptr<Sphere>(new Sphere{1.0f, 25u, 25u, color}); // NOLINT
      scene.getRegistry().attachComponent<MeshComponent>(entity, mesh);
      return {};
    }
  };
};
} // namespace redoom::ecs::components
