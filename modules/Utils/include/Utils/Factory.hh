#pragma once

#include <filesystem>
#include <functional>
#include <memory>
#include <stdexcept>
#include <type_traits>

#include <yaml-cpp/yaml.h>

#include <Utils/Expected.hh>
#include <Utils/Serializer.hh>

namespace redoom::Utils
{
template <typename T, typename Serialize, typename Deserialize>
class Factory
{
public:
  using value_type = T;
  using serialize_type = Serialize;
  using deserialize_type = Deserialize;
  using serializer_type = Serializer<T, Serialize, Deserialize>;

  Factory() noexcept = default;
  virtual ~Factory() noexcept = default;

  template <typename... Args>
  auto serialize(std::string_view type, Args&&... args) const
  {
    auto serializer_exp = this->getSerializer(type);
    if (!serializer_exp)
      throw std::runtime_error{serializer_exp.error()};
    auto const& serializer = (*serializer_exp).get();
    return serializer.serialize(std::forward<Args>(args)...);
  }

  template <typename... Args>
  auto deserialize(std::string_view type, Args&&... args) const
  {
    auto serializer_exp = this->getSerializer(type);
    if (!serializer_exp)
      throw std::runtime_error{serializer_exp.error()};
    auto const& serializer = (*serializer_exp).get();
    return serializer.deserialize(std::forward<Args>(args)...);
  }

  [[nodiscard]] virtual Expected<> loadSerializer(
      std::filesystem::path const& path, std::string_view key) const
  {
    auto dll_exp = DLLoader::get().getLibrary(path);
    RETURN_IF_UNEXPECTED(dll_exp);
    auto const& dll = (*dll_exp).get();
    auto serializer_exp =
        serializer_type::fromDL(dll, "serialize", "deserialize");
    RETURN_IF_UNEXPECTED(serializer_exp);
    auto [it, success] = this->serializers_.emplace(
        key.data(), SerializerData{dll, std::move(*serializer_exp)});
    if (!success)
      return make_formatted_unexpected(
          "Dynamic library {} not found", path.c_str());
    return {};
  }

protected:
  struct SerializerData {
    DynLibrary const& dll;
    serializer_type serializer;
  };

  Expected<std::reference_wrapper<serializer_type>> getSerializer(
      std::string_view key) const noexcept
  {
    auto serializer_it = this->serializers_.find(key.data());
    if (serializer_it == this->serializers_.end()) {
      auto load_exp = this->loadSerializer(key, key);
      RETURN_IF_UNEXPECTED(load_exp);
      return this->getSerializer(key);
    } else {
      return serializer_it->second.serializer;
    }
  }

  mutable std::unordered_map<std::string, SerializerData> serializers_{};
};
} // namespace redoom::Utils
