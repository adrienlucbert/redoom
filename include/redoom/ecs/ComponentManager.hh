#pragma once

#include <algorithm>
#include <mutex>
#include <type_traits>
#include <unordered_map>

#include <tl/optional.hpp>

#include <redoom/ecs/Component.hh>
#include <redoom/ecs/ComponentBase.hh>
#include <redoom/ecs/Entity.hh>
#include <redoom/memory/Allocator.hh>

namespace redoom::ecs
{
using memory::Allocator;
using memory::AllocatorBase;

class ComponentManager
{
public:
  ComponentManager() noexcept = default;
  ComponentManager(ComponentManager const& b) noexcept = delete;
  ComponentManager(ComponentManager&& b) noexcept = default;
  ~ComponentManager() noexcept = default;

  ComponentManager& operator=(ComponentManager const& rhs) noexcept = delete;
  ComponentManager& operator=(ComponentManager&& rhs) noexcept = default;

  template <typename T, typename... Args>
  T& make(Entity entity, Args&&... args) noexcept
  {
    auto& allocator = this->getAllocator<T>();
    auto& list = this->getComponentsList<T>();
    auto lock = std::lock_guard{*this->mutex_};
    auto const& component_it =
        list.emplace(entity, allocator.get(std::forward<Args>(args)...)).first;
    return static_cast<T&>(*component_it->second);
  }

  void releaseAll(Entity entity) noexcept
  {
    for (auto& [type_id, list] : this->components_lists_) {
      auto const& component_it = list.find(entity);
      if (component_it != list.end())
        list.erase(component_it);
    }
  }

  template <typename T>
  bool release(Entity entity) noexcept
  {
    static_assert(std::is_base_of_v<ComponentBase, T>,
        "T must inherit from ComponentBase");
    auto& list = this->getComponentsList<T>();
    auto lock = std::lock_guard{*this->mutex_};
    auto const& component_it = list.find(entity);
    if (component_it == list.end())
      return false;
    else {
      list.erase(component_it);
      return true;
    }
  }

  template <typename T>
  [[nodiscard]] bool has(Entity entity) const noexcept
  {
    static_assert(std::is_base_of_v<ComponentBase, T>,
        "T must inherit from ComponentBase");
    auto lock = std::lock_guard{*this->mutex_};
    auto const& list_it = this->components_lists_.find(T::getTypeId());

    if (list_it == this->components_lists_.end())
      return false;
    auto const& list = this->components_lists_.at(T::getTypeId());
    return list.contains(entity);
  }

  template <typename T, typename Callable>
  void apply(Callable f) noexcept
  {
    auto& list = this->getComponentsList<T>();
    for (auto& component : list)
      f(component.first, static_cast<T&>(*component.second));
  }

  template <typename T>
  tl::optional<T const&> get(Entity entity) const noexcept
  {
    static_assert(std::is_base_of_v<ComponentBase, T>,
        "T must inherit from ComponentBase");
    auto opt = this->getByTypeId(T::getTypeId(), entity);
    return opt.has_value() ? tl::optional<T const&>(static_cast<T const&>(*opt))
                           : tl::nullopt;
  }

  template <typename T>
  tl::optional<T&> get(Entity entity) noexcept
  {
    static_assert(std::is_base_of_v<ComponentBase, T>,
        "T must inherit from ComponentBase");
    auto opt = this->getByTypeId(T::getTypeId(), entity);
    return opt.has_value() ? tl::optional<T&>(static_cast<T&>(*opt))
                           : tl::nullopt;
  }

  template <typename T>
  [[nodiscard]] tl::optional<T const&> getOne() noexcept
  {
    static_assert(std::is_base_of_v<ComponentBase, T>,
        "T must inherit from ComponentBase");
    auto lock = std::lock_guard{*this->mutex_};
    auto list_it = this->components_lists_.find(T::getTypeId());
    if (list_it == this->components_lists_.end())
      return tl::nullopt;
    auto const& list = (*list_it).second;
    auto component_it = std::find_if(
        list.begin(), list.end(), [&](auto const&) { return true; });
    if (component_it == list.end())
      return tl::nullopt;
    else
      return static_cast<T const&>(*component_it->second);
  }

  tl::optional<ComponentBase const&> getByTypeId(
      unsigned int type_id, Entity entity) const noexcept
  {
    auto lock = std::lock_guard{*this->mutex_};
    auto list_it = this->components_lists_.find(type_id);
    if (list_it == this->components_lists_.end())
      return tl::nullopt;
    auto const& list = this->components_lists_.at(type_id);
    auto component_it = list.find(entity);
    if (component_it == list.end())
      return tl::nullopt;
    else
      return *component_it->second;
  }

  tl::optional<ComponentBase&> getByTypeId(
      unsigned int type_id, Entity entity) noexcept
  {
    auto opt =
        const_cast<ComponentManager const*>(this)->getByTypeId(type_id, entity);
    return opt.has_value()
             ? tl::optional<ComponentBase&>(const_cast<ComponentBase&>(*opt))
             : tl::nullopt;
  }

private:
  template <typename T>
  std::unordered_map<Entity, typename Allocator<ComponentBase>::ptr_t>&
  getComponentsList() noexcept
  {
    static_assert(std::is_base_of_v<ComponentBase, T>,
        "T must inherit from ComponentBase");
    auto lock = std::lock_guard{*this->mutex_};
    auto const& list_it = this->components_lists_.find(T::getTypeId());

    if (list_it == this->components_lists_.end())
      this->components_lists_.emplace(T::getTypeId(),
          std::unordered_map<unsigned int, Allocator<ComponentBase>::ptr_t>{});
    return this->components_lists_.at(T::getTypeId());
  }

  template <typename T>
  Allocator<T>& getAllocator() noexcept
  {
    static_assert(std::is_base_of_v<ComponentBase, T>,
        "T must inherit from ComponentBase");
    auto const key = T::getTypeId();
    auto lock = std::lock_guard{*this->mutex_};
    auto allocator_it = this->allocators_.find(key);
    if (allocator_it != this->allocators_.end())
      return *dynamic_cast<Allocator<T>*>(allocator_it->second.get());
    return *dynamic_cast<Allocator<T>*>(
        this->allocators_.emplace(key, std::make_unique<Allocator<T>>())
            .first->second.get());
  }

  mutable std::unique_ptr<std::mutex> mutex_{std::make_unique<std::mutex>()};
  std::unordered_map<unsigned int, std::unique_ptr<AllocatorBase>> allocators_;
  std::unordered_map<std::string, unsigned int> type_type_id_map_;
  std::unordered_map<unsigned int,
      std::unordered_map<Entity, Allocator<ComponentBase>::ptr_t>>
      components_lists_;
};
} // namespace redoom::ecs
