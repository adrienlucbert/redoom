#pragma once

#include <cassert>
#include <cstdlib>
#include <memory>
#include <type_traits>

#include <Utils/traits.hpp>

namespace redoom::Utils
{
template <typename T>
class Singleton
{
public:
  Singleton(Singleton const& b) noexcept = delete;
  Singleton(Singleton&& b) noexcept = delete;
  virtual ~Singleton() noexcept = default;

  Singleton& operator=(Singleton const& rhs) noexcept = delete;
  Singleton& operator=(Singleton&& rhs) noexcept = delete;

  template <typename... Args>
  static T& create(Args&&... args) noexcept
  {
    static_assert(
        Utils::is_constructible_v<T,
            Args...> && "T must be constructible with the given arguments");
    assert(Singleton<T>::instance_ == nullptr
           && "T cannot be instanciated more than once");
    Singleton<T>::instance_ = static_cast<T*>(
        new detail::Derived<T>{std::forward<Args>(args)...}); // NOLINT
    std::atexit([]() {
      delete Singleton<T>::instance_; // NOLINT
      Singleton<T>::instance_ = nullptr;
    });
    return *Singleton<T>::instance_;
  }

  [[nodiscard]] static T& get() noexcept
  {
    if (Singleton<T>::instance_ == nullptr) {
      if constexpr (!Utils::is_default_constructible_v<T>) {
        assert(
            "T cannot be default-constructed. Use `create` method." == nullptr);
        std::abort();
      } else {
        return Singleton<T>::create();
      }
    } else {
      return *Singleton<T>::instance_;
    }
  }

protected:
  Singleton() noexcept
  {
    Singleton<T>::instance_ = static_cast<T*>(this);
  }

private:
  static T* instance_; // NOLINT
};

template <typename T>
T* Singleton<T>::instance_ = nullptr; // NOLINT
} // namespace redoom::Utils
