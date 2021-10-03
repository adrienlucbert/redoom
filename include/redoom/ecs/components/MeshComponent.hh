#pragma once

#include <memory>

#include <redoom/Scene.hh>
#include <redoom/ecs/Component.hh>
#include <redoom/graphics/Mesh.hh>
#include <redoom/graphics/mesh/Quad.hh>
#include <redoom/serializer/ComponentSerializer.hh>

namespace redoom::ecs::components
{
using redoom::graphics::mesh::Quad;

struct MeshComponent : public Component<MeshComponent> {
  [[nodiscard]] std::string const& getType() const noexcept override
  {
    static auto const type = std::string{"MeshComponent"};
    return type;
  }

  explicit MeshComponent(std::shared_ptr<graphics::Mesh> pmesh)
    : mesh{std::move(pmesh)}
  {
  }

  std::shared_ptr<graphics::Mesh> mesh;

  struct Serializer : public ComponentSerializer {
    void serialize(YAML::Emitter& /*out*/,
        ecs::ComponentBase const* /*component*/) const override
    {
      // TODO(alucbert): serialize meshes
      // auto const* mc = dynamic_cast<MeshComponent const*>(component);
    }

    [[nodiscard]] Expected<> deserialize(
        YAML::Node const& /*node*/, Scene& scene, Entity entity) const override
    {
      // TODO(alucbert): deserialize meshes
      auto quad_mesh = std::shared_ptr<Quad>(new Quad{10.0f, 10.0f}); // NOLINT
      scene.getRegistry().attachComponent<MeshComponent>(entity, quad_mesh);
      return {};
    }
  };
};
} // namespace redoom::ecs::components
