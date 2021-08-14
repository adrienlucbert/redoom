#pragma once

#include <redoom/ecs/ComponentManager.hh>
#include <redoom/ecs/EntityManager.hh>
#include <redoom/ecs/SystemManager.hh>

namespace redoom::ecs
{
class Registry
{
public:
  Registry() noexcept = default;
  Registry(Registry const& b) noexcept = delete;
  Registry(Registry&& b) noexcept = default;
  ~Registry() noexcept = default;

  Registry& operator=(Registry const& rhs) noexcept = delete;
  Registry& operator=(Registry&& rhs) noexcept = default;

  [[nodiscard]] Entity makeEntity() noexcept;
  void releaseEntity(Entity entity) noexcept;
  [[nodiscard]] bool hasEntity(Entity entity) const noexcept;

  template <typename C, typename... Args>
  void attachComponent(Entity entity, Args&&... args) noexcept
  {
    this->component_manager.make<C>(entity, std::forward<Args>(args)...);
  }
  template <typename C>
  void detachComponent(Entity entity) noexcept
  {
    this->component_manager.release<C>(entity);
  }
  template <typename C>
  [[nodiscard]] bool hasComponent(Entity entity) const noexcept
  {
    return this->component_manager.has<C>(entity);
  }

  template <typename T, typename... Args>
  void attachSystem(Args&&... args) noexcept
  {
    this->system_manager.make<T>(std::forward<Args>(args)...);
  }
  template <typename T>
  void detachSystem() noexcept
  {
    this->system_manager.release<T>();
  }
  template <typename T>
  [[nodiscard]] bool hasSystem() const noexcept
  {
    return this->system_manager.has<T>();
  }

  void update(long elapsed_time) noexcept;

private:
  ComponentManager component_manager;
  EntityManager entity_manager;
  SystemManager system_manager;
};
} // namespace redoom::ecs
