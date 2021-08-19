#pragma once

#include <redoom/ecs/ComponentManager.hh>
#include <redoom/ecs/EntityManager.hh>
#include <redoom/ecs/SystemManager.hh>
#include <redoom/ecs/components/BehaviourComponent.hh>
#include <redoom/renderer/Window.hh>

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
    auto context = this->getContext();
    if constexpr (std::is_base_of_v<Behaviour, C>) {
      auto& component =
          this->component_manager.make<components::BehaviourComponent>(
              entity, std::make_unique<C>(std::forward<Args>(args)...));
      component.onInit(entity, context);
    } else {
      auto& component =
          this->component_manager.make<C>(entity, std::forward<Args>(args)...);
      component.onInit(entity, context);
    }
  }

  template <typename C>
  void detachComponent(Entity entity) noexcept
  {
    auto opt = this->component_manager.get<C>(entity);
    if (opt) {
      auto context = this->getContext();
      opt->onDestroy(entity, context);
      this->component_manager.release<C>(entity);
    }
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
  template <typename T, typename... Args>
  void attachSystem(SystemPriority priority, Args&&... args) noexcept
  {
    this->system_manager.make<T>(priority, std::forward<Args>(args)...);
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

  void update(renderer::Window& window, double elapsed_time) noexcept;

private:
  [[nodiscard]] Context getContext() noexcept;
  [[nodiscard]] UpdateContext getUpdateContext(
      renderer::Window& window, double elapsed_time) noexcept;

  ComponentManager component_manager;
  EntityManager entity_manager;
  SystemManager system_manager;
};
} // namespace redoom::ecs
