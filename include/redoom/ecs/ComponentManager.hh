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
  ComponentManager(ComponentManager&& b) noexcept = delete;
  ~ComponentManager() noexcept = default;

  ComponentManager& operator=(ComponentManager const& rhs) noexcept = delete;
  ComponentManager& operator=(ComponentManager&& rhs) noexcept = delete;

  template <typename T, typename... Args>
  T& make(unsigned int entity_id, Args&&... args) noexcept
  {
    auto& allocator = this->getAllocator<T>();
    auto& list = this->getComponentsList<T>();
    auto lock = std::lock_guard{this->mutex};
    auto const& component_it =
        list.emplace(entity_id, allocator.get(std::forward<Args>(args)...))
            .first;
    return static_cast<T&>(*component_it->second);
  }

  // TODO(alucbert): read about time complexity between map/unordered_map and
  // find/find_if/operator[]
  // choose accordingly the best option
  // might be binary_search over map
  // https://thispointer.com/map-vs-unordered_map-when-to-choose-one-over-another/#:~:text=Even%20in%20worst%20case%20it,keys%20are%20in%20same%20bucket).
  template <typename T>
  void release(T& component) noexcept
  {
    static_assert(std::is_base_of_v<ComponentBase, T>,
        "T must inherit from ComponentBase");
    auto& list = this->getComponentsList<T>();
    auto lock = std::lock_guard{this->mutex};
    auto const& component_it =
        std::find_if(list.begin(), list.end(), [&](auto const& pair) {
          return std::addressof(*pair.second) == std::addressof(component);
        });
    if (component_it != list.end())
      list.erase(component_it);
  }

private:
  template <typename T>
  std::unordered_map<unsigned int, typename Allocator<ComponentBase>::ptr_t>&
  getComponentsList() noexcept
  {
    static_assert(std::is_base_of_v<ComponentBase, T>,
        "T must inherit from ComponentBase");
    auto lock = std::lock_guard{this->mutex};
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
    auto lock = std::lock_guard{this->mutex};
    auto allocator_it = this->allocators.find(key);
    if (allocator_it != this->allocators.end())
      return *dynamic_cast<Allocator<T>*>(allocator_it->second.get());
    return *dynamic_cast<Allocator<T>*>(
        this->allocators.emplace(key, std::make_unique<Allocator<T>>())
            .first->second.get());
  }

  mutable std::mutex mutex;
  std::unordered_map<unsigned int, std::unique_ptr<AllocatorBase>> allocators;
  std::unordered_map<unsigned int,
      std::unordered_map<unsigned int, Allocator<ComponentBase>::ptr_t>>
      components_lists;
};
} // namespace redoom::ecs
