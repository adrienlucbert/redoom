#pragma once

#include <yaml-cpp/yaml.h>

#include <memory>

#include <Utils/Expected.hh>
#include <glm/gtc/type_ptr.hpp>
#include <redoom/graphics/Mesh.hh>
#include <redoom/serializer/common.hh>

extern "C" {
void serialize(YAML::Emitter&, std::shared_ptr<redoom::graphics::Mesh> const&);

redoom::Expected<std::shared_ptr<redoom::graphics::Mesh>> deserialize(
    YAML::Node const&);
}
