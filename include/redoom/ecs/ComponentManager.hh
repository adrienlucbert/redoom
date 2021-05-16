#pragma once

#include <memory>
#include <mutex>
#include <stdexcept>
#include <type_traits>
#include <unordered_map>
#include <vector>

#include <redoom/ecs/Component.hh>
#include <redoom/ecs/ComponentBase.hh>
#include <redoom/memory/Allocator.hh>
#include <redoom/memory/IAllocator.hh>

namespace redoom::ecs
{
using memory::Allocator;
using memory::IAllocator;

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
        list.emplace(entity_id,
                static_cast<T*>(allocator.get(std::forward<Args>(args)...)))
            .first;
    return static_cast<T&>(*component_it->second);
  }

  template <typename T>
  void release(unsigned int entity_id) noexcept
  {
    static_assert(std::is_base_of_v<ComponentBase, T>,
        "T must inherit from ComponentBase");
    auto& allocator = this->getAllocator<T>();
    auto& list = this->getComponentsList<T>();
    auto lock = std::lock_guard{this->mutex};
    auto const& component_it = list.find(entity_id);
    if (component_it != list.end()) {
      allocator.release(component_it->second);
      list.erase(component_it);
    }
  }

private:
  template <typename T>
  IAllocator& getAllocator() noexcept
  {
    static_assert(std::is_base_of_v<ComponentBase, T>,
        "T must inherit from ComponentBase");
    auto lock = std::lock_guard{this->mutex};
    auto const& allocator_it = this->allocators.find(T::getTypeId());
    if (allocator_it == this->allocators.end()) {
      this->allocators.emplace(
          T::getTypeId(), std::make_unique<Allocator<T>>());
    }
    return *this->allocators.at(T::getTypeId());
  }

  template <typename T>
  std::unordered_map<unsigned int, ComponentBase*>& getComponentsList() noexcept
  {
    static_assert(std::is_base_of_v<ComponentBase, T>,
        "T must inherit from ComponentBase");
    auto lock = std::lock_guard{this->mutex};
    auto const& list_it = this->components.find(T::getTypeId());

    if (list_it == this->components.end()) {
      this->components.emplace(
          T::getTypeId(), std::unordered_map<unsigned int, ComponentBase*>{});
    }
    return this->components.at(T::getTypeId());
  }

  mutable std::mutex mutex;
  std::unordered_map<unsigned int, std::unique_ptr<IAllocator>> allocators;
  std::unordered_map<unsigned int,
      std::unordered_map<unsigned int, ComponentBase*>>
      components;
};
} // namespace redoom::ecs
