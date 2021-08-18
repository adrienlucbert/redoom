#pragma once

#include <algorithm>
#include <cassert>
#include <map>
#include <memory>
#include <mutex>
#include <type_traits>

#include <Utils/UniqueTypeId.hh>
#include <redoom/ecs/ComponentManager.hh>
#include <redoom/ecs/System.hh>
#include <redoom/ecs/SystemBase.hh>
#include <redoom/ecs/UpdateContext.hh>

namespace redoom::ecs
{
struct SystemPriority {
  unsigned int priority;
};

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
  void make(SystemPriority priority, Args&&... args) noexcept
  {
    static_assert(
        std::is_base_of_v<System<T>, T>, "T must inherit from System<T>");
    auto lock = std::lock_guard{*this->mutex};
    auto data = SystemData{
        T::getTypeId(), std::make_unique<T>(std::forward<Args>(args)...)};
    this->systems.emplace(priority.priority, std::move(data));
  }

  template <typename T, typename... Args>
  void make(Args&&... args) noexcept
  {
    this->make<T>(SystemPriority{0}, std::forward<Args>(args)...);
  }

  template <typename T>
  void setPriority(SystemPriority priority) noexcept
  {
    static_assert(
        std::is_base_of_v<System<T>, T>, "T must inherit from System<T>");
    auto system_it = this->find<T>();
    if (system_it != this->systems.end()) {
      auto data = std::move(this->systems.extract(system_it));
      auto lock = std::lock_guard{*this->mutex};
      this->systems.emplace(priority, std::move(data));
    }
  }

  template <typename T>
  void release() noexcept
  {
    static_assert(
        std::is_base_of_v<System<T>, T>, "T must inherit from System<T>");
    auto const& system_it = this->find<T>();
    if (system_it != this->systems.end()) {
      auto lock = std::lock_guard{*this->mutex};
      this->systems.erase(system_it);
    } else
      assert("Exactly one element should be released" == nullptr);
  }

  template <typename T>
  [[nodiscard]] bool has() const noexcept
  {
    return this->find<T>() != this->systems.end();
  }

  void update(UpdateContext& context) noexcept;

private:
  struct SystemData {
    Utils::type_id type_id;
    std::unique_ptr<SystemBase> system;
  };

  template <typename T>
  auto find() const noexcept
  {
    static_assert(
        std::is_base_of_v<System<T>, T>, "T must inherit from System<T>");
    auto lock = std::lock_guard{*this->mutex};
    return std::find_if(this->systems.begin(),
        this->systems.end(),
        [](auto const& pair) { return pair.second.type_id == T::getTypeId(); });
  }

  mutable std::unique_ptr<std::mutex> mutex{std::make_unique<std::mutex>()};
  std::multimap<int, SystemData> systems;
};
} // namespace redoom::ecs
