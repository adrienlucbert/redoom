#pragma once

#include <redoom/ecs/ComponentManager.hh>
#include <redoom/ecs/EntityManager.hh>
#include <redoom/ecs/SystemManager.hh>
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
    this->component_manager_.make<C>(entity, std::forward<Args>(args)...);
  }

  template <typename C>
  void detachComponent(Entity entity) noexcept
  {
    auto opt = this->component_manager_.get<C>(entity);
    if (opt)
      this->component_manager_.release<C>(entity);
  }
  template <typename C>
  [[nodiscard]] bool hasComponent(Entity entity) const noexcept
  {
    return this->component_manager_.has<C>(entity);
  }
  template <typename C>
  [[nodiscard]] tl::optional<C&> getComponent(Entity entity) noexcept
  {
    return this->component_manager_.get<C>(entity);
  }
  template <typename C>
  [[nodiscard]] tl::optional<C const&> getComponent(
      Entity entity) const noexcept
  {
    return this->component_manager_.get<C>(entity);
  }
  [[nodiscard]] tl::optional<ComponentBase&> getComponentByTypeId(
      unsigned type_id, Entity entity) noexcept
  {
    return this->component_manager_.getByTypeId(type_id, entity);
  }
  [[nodiscard]] tl::optional<ComponentBase const&> getComponentByTypeId(
      unsigned type_id, Entity entity) const noexcept
  {
    return this->component_manager_.getByTypeId(type_id, entity);
  }

  template <typename C, typename Callable>
  void apply(Callable f) noexcept
  {
    this->component_manager_.apply<C>(f);
  }

  template <typename T, typename... Args>
  void attachSystem(Args&&... args) noexcept
  {
    this->system_manager_.make<T>(std::forward<Args>(args)...);
  }
  template <typename T, typename... Args>
  void attachSystem(SystemPriority priority, Args&&... args) noexcept
  {
    this->system_manager_.make<T>(priority, std::forward<Args>(args)...);
  }
  template <typename T>
  void detachSystem() noexcept
  {
    this->system_manager_.release<T>();
  }
  template <typename T>
  [[nodiscard]] bool hasSystem() const noexcept
  {
    return this->system_manager_.has<T>();
  }

  void init() noexcept;

  void update(renderer::Window& window, double elapsed_time) noexcept;

  std::vector<Entity> const& getEntities() const noexcept
  {
    return this->entity_manager_.getEntities();
  }

private:
  [[nodiscard]] Context getContext() noexcept;
  [[nodiscard]] UpdateContext getUpdateContext(
      renderer::Window& window, double elapsed_time) noexcept;

  bool is_init_{false};
  ComponentManager component_manager_;
  EntityManager entity_manager_;
  SystemManager system_manager_;
};
} // namespace redoom::ecs
