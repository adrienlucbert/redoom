#pragma once

#include <concepts>
#include <iterator>

namespace redoom::graphics
{
class Program;
} // namespace redoom::graphics

namespace redoom::concepts
{
// clang-format off
template <typename T, typename ValueType = T>
concept Container = requires(T v)
{
  requires std::same_as<typename T::value_type, ValueType>;
  typename T::value_type;
  typename T::iterator;
  requires std::input_or_output_iterator<typename T::iterator>;
  { v.begin() } -> std::same_as<typename T::iterator>;
  { v.end() } -> std::same_as<typename T::iterator>;
  { v.size() } -> std::same_as<std::size_t>;
};

template <typename T, typename ValueType = T>
concept ContiguousContainer = requires(T v)
{
  requires Container<T, ValueType>;
  { v.data() } -> std::same_as<typename T::value_type*>;
};

template <typename T>
concept Drawable = requires(T v, graphics::Program &p)
{
  { v.draw(p) } noexcept -> std::same_as<void>;
};

// clang-format on
} // namespace redoom::concepts
