#pragma once

#include <algorithm>
#include <cassert>
#include <map>
#include <memory>
#include <mutex>
#include <type_traits>

#include <Utils/ThreadPool.hh>
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
  SystemManager(SystemManager const&) noexcept = delete;
  SystemManager(SystemManager&&) noexcept = default;
  ~SystemManager() noexcept;

  SystemManager& operator=(SystemManager const&) noexcept = delete;
  SystemManager& operator=(SystemManager&&) noexcept = default;

  template <typename T, typename... Args>
  void make(SystemPriority priority, Args&&... args) noexcept
  {
    static_assert(std::is_base_of_v<detail::System<T>, T>,
        "T must inherit from System<T>");
    if (this->find<T>() != this->systems_.end()) {
      assert("Cannot make more than one system of the same type" == nullptr);
    } else {
      auto lock = std::lock_guard{*this->mutex_};
      auto data = SystemData{
          T::getTypeId(), std::make_unique<T>(std::forward<Args>(args)...)};
      auto it = this->systems_.emplace(priority.priority, std::move(data));
      it->second.system->onAttach();
      if constexpr (std::is_base_of_v<MultithreadedSystem<T>, T>) {
        this->thread_pool_.reserve(this->thread_pool_.size() + 1);
      }
    }
  }

  template <typename T, typename... Args>
  void make(Args&&... args) noexcept
  {
    this->make<T>(SystemPriority{0}, std::forward<Args>(args)...);
  }

  template <typename T>
  void setPriority(SystemPriority priority) noexcept
  {
    static_assert(std::is_base_of_v<detail::System<T>, T>,
        "T must inherit from System<T>");
    auto system_it = this->find<T>();
    if (system_it != this->systems_.end()) {
      auto data = std::move(this->systems_.extract(system_it));
      auto lock = std::lock_guard{*this->mutex_};
      this->systems_.emplace(priority, std::move(data));
    }
  }

  template <typename T>
  void release() noexcept
  {
    static_assert(std::is_base_of_v<detail::System<T>, T>,
        "T must inherit from System<T>");
    auto const& system_it = this->find<T>();
    if (system_it != this->systems_.end()) {
      auto lock = std::lock_guard{*this->mutex_};
      system_it->second.system->onDetach();
      this->systems_.erase(system_it);
    } else
      assert("Exactly one element should be released" == nullptr);
  }

  template <typename T>
  [[nodiscard]] bool has() const noexcept
  {
    return this->find<T>() != this->systems_.end();
  }

  void update(UpdateContext& context) noexcept;

private:
  void runUpdateHook(void (SystemBase::*hook)(UpdateContext&),
      UpdateContext& context) noexcept;

  struct SystemData {
    Utils::type_id type_id;
    std::unique_ptr<SystemBase> system;
  };

  template <typename T>
  auto find() const noexcept
  {
    static_assert(std::is_base_of_v<detail::System<T>, T>,
        "T must inherit from System<T>");
    auto lock = std::lock_guard{*this->mutex_};
    return std::find_if(this->systems_.begin(),
        this->systems_.end(),
        [](auto const& pair) { return pair.second.type_id == T::getTypeId(); });
  }

  mutable std::unique_ptr<std::mutex> mutex_{std::make_unique<std::mutex>()};
  Utils::ThreadPool thread_pool_;
  std::multimap<int, SystemData> systems_;
};
} // namespace redoom::ecs
