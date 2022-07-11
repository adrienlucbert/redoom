#pragma once

#include <cstdlib>
#include <iostream>
#include <memory>
#include <type_traits>

#include <fmt/format.h>

#include <Utils/traits.hpp>

namespace redoom::Utils
{
template <typename T>
class Singleton
{
public:
  Singleton(Singleton const&) noexcept = delete;
  Singleton(Singleton&&) noexcept = delete;
  virtual ~Singleton() noexcept = default;

  Singleton& operator=(Singleton const&) noexcept = delete;
  Singleton& operator=(Singleton&&) noexcept = delete;

  template <typename... Args>
  static T& create(Args&&... args) noexcept
  {
    static_assert(
        Utils::is_constructible_v<T,
            Args...> && "T must be constructible with the given arguments");
    if (Singleton<T>::instance_ != nullptr) {
      std::cerr << fmt::format(
          "T ({}) cannot be instanciated more than once", typeid(T).name())
                << '\n';
      std::abort();
    }
    Singleton<T>::instance_ = static_cast<T*>(
        new detail::Derived<T>{std::forward<Args>(args)...}); // NOLINT
    (void)std::atexit([]() {
      delete Singleton<T>::instance_; // NOLINT
      Singleton<T>::instance_ = nullptr;
    });
    return *Singleton<T>::instance_;
  }

  [[nodiscard]] static T& get() noexcept
  {
    if (Singleton<T>::instance_ == nullptr) {
      if constexpr (!Utils::is_default_constructible_v<T>) {
        std::cerr << "T cannot be default-constructed. Use `create` method."
                  << '\n';
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
