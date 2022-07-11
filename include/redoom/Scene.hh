#pragma once

#include <redoom/ecs/Registry.hh>
#include <redoom/physics/World.hh>

namespace redoom
{
class SceneSerializer;

class Scene
{
public:
  explicit Scene(std::string_view name) noexcept;
  Scene(Scene const&) noexcept = delete;
  Scene(Scene&&) noexcept = default;
  ~Scene() noexcept = default;

  Scene& operator=(Scene const&) noexcept = delete;
  Scene& operator=(Scene&&) noexcept = default;

  [[nodiscard]] std::string const& getName() const noexcept;
  [[nodiscard]] ecs::Registry& getRegistry() noexcept;
  [[nodiscard]] ecs::Registry const& getRegistry() const noexcept;
  [[nodiscard]] physics::World& getWorld() noexcept;
  [[nodiscard]] physics::World const& getWorld() const noexcept;

  void serialize(std::string_view filepath) const noexcept;

private:
  friend class SceneSerializer;

  void setName(std::string_view name) noexcept;

  std::string name_;
  ecs::Registry registry_;
  physics::World world_;
};
} // namespace redoom
