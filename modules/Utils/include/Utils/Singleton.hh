#pragma once

#include <cassert>
#include <cstdlib>
#include <iostream>
#include <memory>
#include <type_traits>

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
    static_assert(std::is_constructible_v<T, Args...>);
    if (Singleton<T>::instance != nullptr)
      assert("Singleton cannot be instanciated more than once at a time."
             == nullptr);
    Singleton<T>::instance = new T{std::forward<Args>(args)...}; // NOLINT
    std::atexit([]() {
      delete Singleton<T>::instance; // NOLINT
      Singleton<T>::instance = nullptr;
    });
    return *Singleton<T>::instance;
  }

  [[nodiscard]] static T& get() noexcept
  {
    if (Singleton<T>::instance == nullptr) {
      if constexpr (!std::is_default_constructible_v<T>)
        assert("Singleton cannot be default-constructed. Use `create` method."
               == nullptr);
      else
        return Singleton<T>::create();
    } else
      return *Singleton<T>::instance;
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
