#pragma once

#include <memory>
#include <string_view>

#include <yaml-cpp/yaml.h>

#include <Utils/DLLoader.hh>
#include <Utils/Expected.hh>

namespace redoom::Utils
{
template <typename T,
    typename Serialize = void (*)(YAML::Emitter&, T const&),
    typename Deserialize = Expected<T> (*)(YAML::Node const&)>
class Serializer
{
public:
  Expected<Serializer> static fromDL(DynLibrary const& dll,
      std::string_view serialize_symbol,
      std::string_view deserialize_symbol) noexcept
  {
    auto serialize_exp = dll.getSymbol<Serialize>(serialize_symbol);
    RETURN_IF_UNEXPECTED(serialize_exp);
    auto deserialize_exp = dll.getSymbol<Deserialize>(deserialize_symbol);
    RETURN_IF_UNEXPECTED(deserialize_exp);
    return {Serializer{*serialize_exp, *deserialize_exp}};
  }

  Expected<Serializer> static fromDL(std::string_view filepath,
      std::string_view serialize_symbol,
      std::string_view deserialize_symbol) noexcept
  {
    auto dll_exp = DLLoader::get().getLibrary(filepath);
    RETURN_IF_UNEXPECTED(dll_exp);
    auto const& dll = (*dll_exp).get();
    return Serializer<T, Serialize, Deserialize>::fromDL(
        dll, serialize_symbol, deserialize_symbol);
  }

  template <typename... Args>
  auto serialize(Args&&... args) const noexcept
  {
    return std::invoke(this->serialize_, std::forward<Args>(args)...);
  }

  template <typename... Args>
  auto deserialize(Args&&... args) const noexcept
  {
    return std::invoke(this->deserialize_, std::forward<Args>(args)...);
  }

private:
  Serializer(Serialize serialize, Deserialize deserialize) noexcept
    : serialize_{serialize}
    , deserialize_{deserialize}
  {
  }

  Serialize serialize_;
  Deserialize deserialize_;
};
} // namespace redoom::Utils
