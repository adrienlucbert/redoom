#pragma once

#include <unordered_map>

#include <Utils/Factory.hh>
#include <Utils/UniqueTypeId.hh>
#include <redoom/Scene.hh>
#include <redoom/ecs/ComponentBase.hh>

namespace redoom
{
class ComponentFactory
  : public Utils::Factory<ecs::ComponentBase,
        void (*)(YAML::Emitter&, ecs::ComponentBase const*),
        Expected<> (*)(YAML::Node const&, Scene&, ecs::Entity)>
{
public:
  ComponentFactory() noexcept = default;
  ~ComponentFactory() noexcept override = default;

  [[nodiscard]] Expected<> loadSerializer(std::filesystem::path const& path,
      std::string_view key) const noexcept override
  {
    auto exp = Utils::Factory<ComponentFactory::value_type,
        ComponentFactory::serialize_type,
        ComponentFactory::deserialize_type>::loadSerializer(path, key);
    RETURN_IF_UNEXPECTED(exp);
    Utils::DynLibrary const& dll = this->serializers_.at(key.data()).dll;
    auto get_type_id_exp = dll.getSymbol<Utils::type_id (*)()>("getTypeId");
    RETURN_IF_UNEXPECTED(get_type_id_exp);
    auto get_type_id = *get_type_id_exp;
    this->types_ids_.emplace(key.data(), get_type_id());
    return {};
  }

  Expected<Utils::type_id> getTypeId(std::string_view key) const noexcept
  {
    auto it = this->types_ids_.find(key.data());
    if (it == this->types_ids_.end())
      return make_formatted_unexpected("{}: key not found", key);
    return it->second;
  }

protected:
  mutable std::unordered_map<std::string, Utils::type_id> types_ids_;
};
} // namespace redoom
