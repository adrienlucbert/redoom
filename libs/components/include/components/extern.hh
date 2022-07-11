#pragma once

#include <yaml-cpp/yaml.h>

#include <Utils/Expected.hh>
#include <Utils/UniqueTypeId.hh>
#include <redoom/Scene.hh>
#include <redoom/ecs/ComponentBase.hh>
#include <redoom/serializer/common.hh>

extern "C" {
redoom::Utils::type_id getTypeId() noexcept;

void serialize(YAML::Emitter& out, redoom::ecs::ComponentBase const* component);

redoom::Expected<> deserialize(
    YAML::Node const& node, redoom::Scene& scene, redoom::ecs::Entity entity);
}
