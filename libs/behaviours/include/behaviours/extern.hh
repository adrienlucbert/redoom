#pragma once

#include <yaml-cpp/yaml.h>

#include <redoom/Scene.hh>
#include <redoom/ecs/Behaviour.hh>
#include <redoom/ecs/Entity.hh>
#include <redoom/ecs/components/BehaviourComponent.hh>

extern "C" {
void serialize(
    YAML::Emitter&, redoom::ecs::components::BehaviourComponent const*);

redoom::Expected<std::unique_ptr<redoom::ecs::Behaviour>> deserialize(
    YAML::Node const&, redoom::Scene&, redoom::ecs::Entity);
}
