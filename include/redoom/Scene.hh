#pragma once

#include <redoom/ecs/Registry.hh>

namespace redoom
{
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

private:
  std::string name;
  ecs::Registry registry;
};
} // namespace redoom
