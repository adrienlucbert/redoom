#pragma once

#include "redoom/graphics/Model.hh"
#include "redoom/physics/World.hh"
#include <memory>

#include <glm/gtc/type_ptr.hpp>
#include <tl/expected.hpp>

#include <Utils/Expected.hh>
#include <redoom/Application.hh>
#include <redoom/Scene.hh>
#include <redoom/ecs/Component.hh>
#include <redoom/physics/Body.hh>
#include <redoom/physics/Fixture.hh>
#include <redoom/physics/shapes/Cuboid.hh>
#include <redoom/physics/shapes/Sphere.hh>
#include <redoom/serializer/ComponentSerializer.hh>
#include <redoom/serializer/common.hh>

namespace redoom::ecs::components
{
struct BodyComponent : public Component<BodyComponent> {
  [[nodiscard]] std::string const& getType() const noexcept override
  {
    static auto const type = std::string{"BodyComponent"};
    return type;
  }

  explicit BodyComponent(
      physics::World& world, physics::BodyDefinition def) noexcept
    : body{world.createBody(def)}
  {
  }
  explicit BodyComponent(physics::Body& pbody) noexcept
    : body{pbody}
  {
  }
  BodyComponent(BodyComponent const&) noexcept = delete;
  BodyComponent(BodyComponent&&) noexcept = default;
  ~BodyComponent() noexcept override
  {
    this->body.get().getWorld().deleteBody(this->body);
  }

  BodyComponent& operator=(BodyComponent const&) noexcept = delete;
  BodyComponent& operator=(BodyComponent&&) noexcept = delete;

  [[nodiscard]] static BodyComponent fromModel(physics::World& world,
      physics::BodyDefinition def,
      graphics::Model const& model) noexcept
  {
    auto& body = world.createBodyFromModel(def, model);
    return BodyComponent{body};
  }

  std::reference_wrapper<physics::Body> body;

  struct Serializer : public ComponentSerializer {
    static Expected<std::string_view> BodyTypeToString(
        physics::BodyType type) noexcept
    {
      static const auto conversion_map =
          std::unordered_map<physics::BodyType, std::string_view>{
              {physics::BodyType::Dynamic, "Dynamic"},
              {physics::BodyType::Kinematic, "Kinematic"},
              {physics::BodyType::Static, "Static"},
          };
      auto const& type_it = conversion_map.find(type);
      if (type_it != conversion_map.end())
        return type_it->second;
      return tl::make_unexpected("BodyType to string conversion case missing");
    }

    static Expected<physics::BodyType> stringToBodyType(
        std::string_view type) noexcept
    {
      static const auto conversion_map =
          std::unordered_map<std::string_view, physics::BodyType>{
              {"Dynamic", physics::BodyType::Dynamic},
              {"Kinematic", physics::BodyType::Kinematic},
              {"Static", physics::BodyType::Static},
          };
      auto const& type_it = conversion_map.find(type);
      if (type_it != conversion_map.end())
        return type_it->second;
      return make_formatted_unexpected("Invalid BodyType: {}", type);
    }

    static void serializeFixture(
        YAML::Emitter& out, physics::Fixture const& fixture) noexcept
    {
      out << YAML::BeginMap;
      out << YAML::Key << "shape" << YAML::Value << YAML::BeginMap;
      auto exp = Serializer::serializeShape(out, fixture.getShape());
      // TODO(alucbert): RETURN_IF_UNEXPECTED(exp);
      out << YAML::EndMap;
      out << YAML::Key << "position" << YAML::Value << fixture.getPosition();
      out << YAML::Key << "friction" << YAML::Value << fixture.getFriction();
      out << YAML::Key << "restitution" << YAML::Value
          << fixture.getRestitution();
      out << YAML::Key << "density" << YAML::Value << fixture.getDensity();
      out << YAML::EndMap;
    }

    static Expected<> serializeShape(YAML::Emitter& out,
        std::unique_ptr<physics::Shape> const& shape) noexcept
    {
      auto shape_type = shape->getType();
      out << YAML::Key << "type" << YAML::Value << shape_type.data();
      if (shape_type == "Cuboid") {
        auto const& cuboid = dynamic_cast<physics::Cuboid const&>(*shape);
        out << YAML::Key << "width" << YAML::Value << cuboid.getWidth();
        out << YAML::Key << "height" << YAML::Value << cuboid.getHeight();
        out << YAML::Key << "length" << YAML::Value << cuboid.getLength();
      } else if (shape_type == "Sphere") {
        auto const& sphere = dynamic_cast<physics::Sphere const&>(*shape);
        out << YAML::Key << "radius" << YAML::Value << sphere.getRadius();
      } else {
        return make_formatted_unexpected("Unknown shape type: {}", shape_type);
      }
      return {};
    }

    static Expected<> deserializeFixture(
        YAML::Node const& node, physics::Body& body) noexcept
    {
      auto shape_node = node["shape"];
      if (!shape_node)
        return tl::make_unexpected("Missing key: shape");
      auto shape_exp = Serializer::deserializeShape(shape_node);
      RETURN_IF_UNEXPECTED(shape_exp);
      auto fixture_def =
          physics::FixtureDefinition{.shape = std::move(shape_exp.value()),
              .position = node["position"].as<glm::vec3>(),
              .friction = node["friction"].as<float>(),
              .restitution = node["restitution"].as<float>(),
              .density = node["density"].as<float>()};
      body.createFixture(std::move(fixture_def));
      return {};
    }

    static Expected<std::unique_ptr<physics::Shape>> deserializeShape(
        YAML::Node const& node) noexcept
    {
      auto shape_type = node["type"].as<std::string>();
      if (shape_type == "Cuboid") {
        auto width_exp = YAML::exp_get_value<float>(node, "width");
        RETURN_IF_UNEXPECTED(width_exp);
        auto height_exp = YAML::exp_get_value<float>(node, "height");
        RETURN_IF_UNEXPECTED(height_exp);
        auto length_exp = YAML::exp_get_value<float>(node, "length");
        RETURN_IF_UNEXPECTED(length_exp);
        return std::make_unique<physics::Cuboid>(
            width_exp.value(), height_exp.value(), length_exp.value());
      } else if (shape_type == "Sphere") {
        auto radius_exp = YAML::exp_get_value<float>(node, "radius");
        RETURN_IF_UNEXPECTED(radius_exp);
        return std::make_unique<physics::Sphere>(radius_exp.value());
      } else {
        return make_formatted_unexpected("Unknown shape type: {}", shape_type);
      }
    }

    void serialize(
        YAML::Emitter& out, ecs::ComponentBase const* component) const override
    {
      auto const* bc = dynamic_cast<BodyComponent const*>(component);
      auto type_exp = Serializer::BodyTypeToString(bc->body.get().getType());
      if (!type_exp.has_value()) {
        std::cerr << "BodyComponent serialization failed: " << type_exp.error()
                  << '\n';
        return;
      }
      out << YAML::Key << "type" << YAML::Value << type_exp.value().data();
      out << YAML::Key << "position" << YAML::Value
          << bc->body.get().getPosition();
      out << YAML::Key << "angle" << YAML::Value << bc->body.get().getAngle();
      out << YAML::Key << "linear_velocity" << YAML::Value
          << bc->body.get().getLinearVelocity();
      out << YAML::Key << "angular_velocity" << YAML::Value
          << bc->body.get().getAngularVelocity();
      out << YAML::Key << "has_fixed_rotation" << YAML::Value
          << bc->body.get().hasFixedRotation();
      out << YAML::Key << "gravity_scale" << YAML::Value
          << bc->body.get().getGravityScale();
      out << YAML::Key << "fixtures" << YAML::Value << YAML::BeginSeq;
      for (auto const& fixture : bc->body.get().getFixtures()) {
        Serializer::serializeFixture(out, fixture);
      }
      out << YAML::EndSeq;
    }

    [[nodiscard]] Expected<> deserialize(
        YAML::Node const& node, Scene& scene, Entity entity) const override
    {
      auto type_exp =
          Serializer::stringToBodyType(node["type"].as<std::string>());
      RETURN_IF_UNEXPECTED(type_exp);
      auto body_def = physics::BodyDefinition{.type = type_exp.value(),
          .position = node["position"].as<glm::vec3>(),
          .angle = node["angle"].as<float>(),
          .linear_velocity = node["linear_velocity"].as<glm::vec3>(),
          .angular_velocity = node["angular_velocity"].as<float>(),
          .has_fixed_rotation = node["has_fixed_rotation"].as<bool>(),
          .gravity_scale = node["gravity_scale"].as<float>()};
      auto& pbody = scene.getWorld().createBody(body_def);
      auto const fixtures = node["fixtures"];
      if (fixtures) {
        for (auto const& fixture_node : fixtures) {
          Serializer::deserializeFixture(fixture_node, pbody);
        }
      }
      scene.getRegistry().attachComponent<BodyComponent>(
          entity, BodyComponent{pbody});
      return {};
    }
  };
};
} // namespace redoom::ecs::components
