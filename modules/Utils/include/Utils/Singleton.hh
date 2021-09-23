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
    assert(Singleton<T>::instance == nullptr
           && "T cannot be instanciated more than once");
    Singleton<T>::instance = static_cast<T*>(
        new detail::Derived<T>{std::forward<Args>(args)...}); // NOLINT
    std::atexit([]() {
      delete Singleton<T>::instance; // NOLINT
      Singleton<T>::instance = nullptr;
    });
    return *Singleton<T>::instance;
  }

  [[nodiscard]] static T& get() noexcept
  {
    if (Singleton<T>::instance == nullptr) {
      if constexpr (!Utils::is_default_constructible_v<T>) {
        assert(
            "T cannot be default-constructed. Use `create` method." == nullptr);
        std::abort();
      } else {
        return Singleton<T>::create();
      }
    } else {
      return *Singleton<T>::instance;
    }
  }

protected:
  Singleton() noexcept
  {
    Singleton<T>::instance = static_cast<T*>(this);
  }

private:
  static T* instance; // NOLINT
};

template <typename T>
T* Singleton<T>::instance = nullptr; // NOLINT
} // namespace redoom::Utils
