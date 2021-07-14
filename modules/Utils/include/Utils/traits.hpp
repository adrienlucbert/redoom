#pragma once

#include <type_traits>

namespace redoom::Utils
{
template <typename T, typename... Ts>
using are_same =
    typename std::enable_if_t<std::conjunction_v<std::is_same<Ts, T>...>>;
}
