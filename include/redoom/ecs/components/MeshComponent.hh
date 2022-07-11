#pragma once

#include <memory>

#include <Utils/Factory.hh>
#include <redoom/Scene.hh>
#include <redoom/ecs/Component.hh>
#include <redoom/graphics/Mesh.hh>

namespace redoom::ecs::components
{
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
};
} // namespace redoom::ecs::components
