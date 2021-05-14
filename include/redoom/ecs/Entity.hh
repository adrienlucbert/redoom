#pragma once

namespace redoom::ecs
{
class Entity
{
public:
  Entity() noexcept;
  Entity(Entity const& b) noexcept = default;
  Entity(Entity&& b) noexcept = default;
  ~Entity() noexcept = default;

  Entity& operator=(Entity const& rhs) noexcept = default;
  Entity& operator=(Entity&& rhs) noexcept = default;

  [[nodiscard]] unsigned int getId() const noexcept;

private:
  [[nodiscard]] static unsigned int getUniqueId() noexcept;

  unsigned int id;
};
} // namespace redoom::ecs
