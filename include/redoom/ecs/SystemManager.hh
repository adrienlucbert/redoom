#pragma once

#include <memory>
#include <mutex>
#include <type_traits>
#include <vector>

#include <redoom/ecs/System.hh>

namespace redoom::ecs
{
class SystemManager
{
public:
  SystemManager() noexcept = default;
  SystemManager(SystemManager const& b) noexcept = delete;
  SystemManager(SystemManager&& b) noexcept = delete;
  ~SystemManager() noexcept = default;

  SystemManager& operator=(SystemManager const& rhs) noexcept = delete;
  SystemManager& operator=(SystemManager&& rhs) noexcept = delete;

  template <typename T, typename... Args>
  void make(Args&&... args) noexcept
  {
    static_assert(std::is_base_of_v<System, T>, "T must inherit from System");
    auto lock = std::lock_guard{this->mutex};
    this->systems.emplace_back(
        std::make_unique<T>(T{std::forward<Args>(args)...}));
  }

  template <typename T>
  void release(T& system) noexcept
  {
    static_assert(std::is_base_of_v<System, T>, "T must inherit from System");
    auto lock = std::lock_guard{this->mutex};
    auto const& system_it = std::find_if(
        this->systems.begin(), this->systems.end(), [&](auto const& psystem) {
          return std::addressof(*psystem) == std::addressof(system);
        });
    if (system_it != this->systems.end())
      this->systems.erase(system_it);
  }
  void update(long elapsed_time) noexcept;

private:
  mutable std::mutex mutex;
  std::vector<std::unique_ptr<System>> systems;
};
} // namespace redoom::ecs
