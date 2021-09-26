#pragma once

#include <redoom/ecs/Registry.hh>

namespace redoom
{
class SceneSerializer;

class Scene
{
public:
  explicit Scene(std::string_view pname) noexcept;
  Scene(Scene const& b) noexcept = delete;
  Scene(Scene&& b) noexcept = default;
  ~Scene() noexcept = default;

  Scene& operator=(Scene const& rhs) noexcept = delete;
  Scene& operator=(Scene&& rhs) noexcept = default;

  [[nodiscard]] std::string const& getName() const noexcept;
  [[nodiscard]] ecs::Registry& getRegistry() noexcept;
  [[nodiscard]] ecs::Registry const& getRegistry() const noexcept;

  void serialize(std::string_view filepath) const noexcept;

private:
  friend class SceneSerializer;

  void setName(std::string_view pname) noexcept;

  std::string name;
  ecs::Registry registry;
};
} // namespace redoom
