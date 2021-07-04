#pragma once

#include <algorithm>
#include <mutex>
#include <type_traits>
#include <unordered_map>

#include <redoom/ecs/Component.hh>
#include <redoom/ecs/ComponentBase.hh>
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
  [[nodiscard]] T& make(unsigned int entity_id, Args&&... args) noexcept
  {
    auto& allocator = this->getAllocator<T>();
    auto& list = this->getComponentsList<T>();
    auto lock = std::lock_guard{*this->mutex};
    auto const& component_it =
        list.emplace(entity_id, allocator.get(std::forward<Args>(args)...))
            .first;
    return static_cast<T&>(*component_it->second);
  }

  template <typename T>
  void release(T& component) noexcept
  {
    static_assert(std::is_base_of_v<ComponentBase, T>,
        "T must inherit from ComponentBase");
    auto& list = this->getComponentsList<T>();
    auto lock = std::lock_guard{*this->mutex};
    auto const& component_it =
        std::find_if(list.begin(), list.end(), [&](auto const& pair) {
          return std::addressof(*pair.second) == std::addressof(component);
        });
    if (component_it != list.end())
      list.erase(component_it);
    else
      assert("Exactly one element should be released" == nullptr);
  }

private:
  template <typename T>
  std::unordered_map<unsigned int, typename Allocator<ComponentBase>::ptr_t>&
  getComponentsList() noexcept
  {
    static_assert(std::is_base_of_v<ComponentBase, T>,
        "T must inherit from ComponentBase");
    auto lock = std::lock_guard{*this->mutex};
    auto const& list_it = this->components_lists.find(T::getTypeId());

    if (list_it == this->components_lists.end())
      this->components_lists.emplace(T::getTypeId(),
          std::unordered_map<unsigned int, Allocator<ComponentBase>::ptr_t>{});
    return this->components_lists.at(T::getTypeId());
  }

  template <typename T>
  Allocator<T>& getAllocator() noexcept
  {
    static_assert(std::is_base_of_v<ComponentBase, T>,
        "T must inherit from ComponentBase");
    auto const key = T::getTypeId();
    auto lock = std::lock_guard{*this->mutex};
    auto allocator_it = this->allocators.find(key);
    if (allocator_it != this->allocators.end())
      return *dynamic_cast<Allocator<T>*>(allocator_it->second.get());
    return *dynamic_cast<Allocator<T>*>(
        this->allocators.emplace(key, std::make_unique<Allocator<T>>())
            .first->second.get());
  }

  mutable std::unique_ptr<std::mutex> mutex{std::make_unique<std::mutex>()};
  std::unordered_map<unsigned int, std::unique_ptr<AllocatorBase>> allocators;
  std::unordered_map<unsigned int,
      std::unordered_map<unsigned int, Allocator<ComponentBase>::ptr_t>>
      components_lists;
};
} // namespace redoom::ecs
