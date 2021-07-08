#pragma once

#include <algorithm>
#include <cassert>
#include <memory>
#include <mutex>
#include <type_traits>
#include <unordered_map>

#include <Utils/UniqueTypeId.hh>
#include <redoom/ecs/ComponentManager.hh>
#include <redoom/ecs/System.hh>
#include <redoom/ecs/SystemBase.hh>

namespace redoom::ecs
{
class SystemManager
{
public:
  SystemManager() noexcept = default;
  SystemManager(SystemManager const& b) noexcept = delete;
  SystemManager(SystemManager&& b) noexcept = default;
  ~SystemManager() noexcept = default;

  SystemManager& operator=(SystemManager const& rhs) noexcept = delete;
  SystemManager& operator=(SystemManager&& rhs) noexcept = default;

  template <typename T, typename... Args>
  void make(Args&&... args) noexcept
  {
    static_assert(
        std::is_base_of_v<System<T>, T>, "T must inherit from System");
    auto lock = std::lock_guard{*this->mutex};
    this->systems.emplace(
        T::getTypeId(), std::make_unique<T>(T{std::forward<Args>(args)...}));
  }

  template <typename T>
  void release() noexcept
  {
    static_assert(
        std::is_base_of_v<System<T>, T>, "T must inherit from System");
    auto lock = std::lock_guard{*this->mutex};
    auto const& system_it = this->systems.find(T::getTypeId());
    if (system_it != this->systems.end())
      this->systems.erase(system_it);
    else
      assert("Exactly one element should be released" == nullptr);
  }
  void update(long elapsed_time, ComponentManager& component_manager) noexcept;

private:
  mutable std::unique_ptr<std::mutex> mutex{std::make_unique<std::mutex>()};
  std::unordered_map<Utils::type_id, std::unique_ptr<SystemBase>> systems;
};
} // namespace redoom::ecs
