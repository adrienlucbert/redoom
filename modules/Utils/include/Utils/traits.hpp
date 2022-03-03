#pragma once

#include <type_traits>
#include <utility>

namespace redoom::Utils
{
template <typename T, typename... Ts>
using are_same =
    typename std::enable_if_t<std::conjunction_v<std::is_same<Ts, T>...>>;

namespace detail
{
template <typename T>
struct Derived : T {
public:
  template <typename... Args>
  explicit Derived(Args&&... args) noexcept
    : T{std::forward<Args>(args)...}
  {
  }

  template <class, class... Args>
  struct is_t_constructible : std::false_type {
  };

  template <class... Args>
  struct is_t_constructible<std::void_t<decltype(T(std::declval<Args>()...))>,
      Args...> : std::true_type {
  };
};
} // namespace detail

/**
 * Alternative to std::is_constructible that looks for protected constructors
 * as well
 */
template <typename T, typename... Args>
using is_constructible =
    typename detail::Derived<T>::template is_t_constructible<void, Args...>;

template <typename T, typename... Args>
inline constexpr auto is_constructible_v = is_constructible<T, Args...>::value;

/**
 * Alternative to std::is_default_constructible that looks for protected
 * constructors as well
 */
template <typename T>
using is_default_constructible = is_constructible<T>;

template <typename T>
inline constexpr auto is_default_constructible_v =
    is_default_constructible<T>::value;
} // namespace redoom::Utils
