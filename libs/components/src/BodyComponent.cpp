#include <components/extern.hh>

#include <glm/gtc/type_ptr.hpp>

#include <Utils/Serializer.hh>
#include <redoom/ecs/components/BodyComponent.hh>
#include <redoom/ecs/components/TransformComponent.hh>
#include <redoom/physics/shapes/Cuboid.hh>
#include <redoom/physics/shapes/Quad.hh>
#include <redoom/physics/shapes/Sphere.hh>

using redoom::ecs::components::BodyComponent;

redoom::Utils::type_id getTypeId() noexcept
{
  return BodyComponent::getTypeId();
}

static redoom::Expected<std::string_view> BodyTypeToString(
    redoom::physics::BodyType type) noexcept
{
  static const auto conversion_map =
      std::unordered_map<redoom::physics::BodyType, std::string_view>{
          {redoom::physics::BodyType::Dynamic, "Dynamic"},
          {redoom::physics::BodyType::Kinematic, "Kinematic"},
          {redoom::physics::BodyType::Static, "Static"},
      };
  auto const& type_it = conversion_map.find(type);
  if (type_it != conversion_map.end())
    return type_it->second;
  return tl::make_unexpected("BodyType to string conversion case missing");
}

static redoom::Expected<redoom::physics::BodyType> stringToBodyType(
    std::string_view type) noexcept
{
  static const auto conversion_map =
      std::unordered_map<std::string_view, redoom::physics::BodyType>{
          {"Dynamic", redoom::physics::BodyType::Dynamic},
          {"Kinematic", redoom::physics::BodyType::Kinematic},
          {"Static", redoom::physics::BodyType::Static},
      };
  auto const& type_it = conversion_map.find(type);
  if (type_it != conversion_map.end())
    return type_it->second;
  return redoom::make_formatted_unexpected("Invalid BodyType: {}", type);
}

static redoom::Expected<> serializeShape(
    YAML::Emitter& out, redoom::physics::Shape const& shape)
{
  auto shape_type = shape.getType();
  out << YAML::Key << "type" << YAML::Value << shape_type.data();
  if (shape_type == "Cuboid") {
    auto const& cuboid = dynamic_cast<redoom::physics::Cuboid const&>(shape);
    out << YAML::Key << "width" << YAML::Value << cuboid.getWidth();
    out << YAML::Key << "height" << YAML::Value << cuboid.getHeight();
    out << YAML::Key << "length" << YAML::Value << cuboid.getLength();
  } else if (shape_type == "Quad") {
    auto const& quad = dynamic_cast<redoom::physics::Quad const&>(shape);
    out << YAML::Key << "width" << YAML::Value << quad.getWidth();
    out << YAML::Key << "height" << YAML::Value << quad.getHeight();
  } else if (shape_type == "Sphere") {
    auto const& sphere = dynamic_cast<redoom::physics::Sphere const&>(shape);
    out << YAML::Key << "radius" << YAML::Value << sphere.getRadius();
  } else {
    return redoom::make_formatted_unexpected(
        "Unknown shape type: {}", shape_type);
  }
  return {};
}

static void serializeFixture(
    YAML::Emitter& out, redoom::physics::Fixture const& fixture)
{
  out << YAML::BeginMap;
  out << YAML::Key << "shape" << YAML::Value << YAML::BeginMap;
  auto exp = serializeShape(out, *fixture.getShape());
  out << YAML::EndMap;
  out << YAML::Key << "local_position" << YAML::Value
      << fixture.getLocalPosition();
  out << YAML::Key << "friction" << YAML::Value << fixture.getFriction();
  out << YAML::Key << "restitution" << YAML::Value << fixture.getRestitution();
  out << YAML::Key << "density" << YAML::Value << fixture.getDensity();
  out << YAML::EndMap;
}

static redoom::Expected<std::shared_ptr<redoom::physics::Shape>>
deserializeShape(YAML::Node const& node) // NOLINT
{
  // TODO(alucbert): reduce cognitive complexity
  auto shape_type = node["type"].as<std::string>();
  if (shape_type == "Cuboid") {
    auto width_exp = YAML::exp_get_value<float>(node, "width");
    RETURN_IF_UNEXPECTED(width_exp);
    auto height_exp = YAML::exp_get_value<float>(node, "height");
    RETURN_IF_UNEXPECTED(height_exp);
    auto length_exp = YAML::exp_get_value<float>(node, "length");
    RETURN_IF_UNEXPECTED(length_exp);
    return std::make_shared<redoom::physics::Cuboid>(
        width_exp.value(), height_exp.value(), length_exp.value());
  } else if (shape_type == "Quad") {
    auto width_exp = YAML::exp_get_value<float>(node, "width");
    RETURN_IF_UNEXPECTED(width_exp);
    auto height_exp = YAML::exp_get_value<float>(node, "height");
    RETURN_IF_UNEXPECTED(height_exp);
    return std::make_shared<redoom::physics::Quad>(
        width_exp.value(), height_exp.value());
  } else if (shape_type == "Sphere") {
    auto radius_exp = YAML::exp_get_value<float>(node, "radius");
    RETURN_IF_UNEXPECTED(radius_exp);
    return std::make_shared<redoom::physics::Sphere>(radius_exp.value());
  } else {
    return redoom::make_formatted_unexpected(
        "Unknown shape type: {}", shape_type);
  }
}

static redoom::Expected<> deserializeFixture(
    YAML::Node const& node, redoom::physics::Body& body)
{
  auto shape_node = node["shape"];
  if (!shape_node)
    return tl::make_unexpected("Missing key: shape");
  auto shape_exp = deserializeShape(shape_node);
  RETURN_IF_UNEXPECTED(shape_exp);
  auto fixture_def =
      redoom::physics::FixtureDefinition{.shape = std::move(shape_exp.value()),
          .local_position = node["local_position"].as<glm::vec3>(),
          .friction = node["friction"].as<float>(),
          .restitution = node["restitution"].as<float>(),
          .density = node["density"].as<float>()};
  body.createFixture(std::move(fixture_def));
  return {};
}

void serialize(YAML::Emitter& out, redoom::ecs::ComponentBase const* component)
{
  auto const* bc = dynamic_cast<BodyComponent const*>(component);
  auto type_exp = BodyTypeToString(bc->body_->getType());
  if (!type_exp.has_value()) {
    std::cerr << "BodyComponent serialization failed: " << type_exp.error()
              << '\n';
    return;
  }
  out << YAML::Key << "type" << YAML::Value << type_exp.value().data();
  out << YAML::Key << "linear_velocity" << YAML::Value
      << bc->body_->getLinearVelocity();
  out << YAML::Key << "angular_velocity" << YAML::Value
      << bc->body_->getAngularVelocity();
  out << YAML::Key << "has_fixed_rotation" << YAML::Value
      << bc->body_->hasFixedRotation();
  out << YAML::Key << "gravity_scale" << YAML::Value
      << bc->body_->getGravityScale();
  out << YAML::Key << "fixtures" << YAML::Value << YAML::BeginSeq;
  for (auto const& fixture : bc->body_->getFixtures()) {
    serializeFixture(out, fixture);
  }
  out << YAML::EndSeq;
}

redoom::Expected<> deserialize(
    YAML::Node const& node, redoom::Scene& scene, redoom::ecs::Entity entity)
{
  auto type_exp = stringToBodyType(node["type"].as<std::string>());
  RETURN_IF_UNEXPECTED(type_exp);
  auto transform_component_opt =
      scene.getRegistry()
          .getComponent<redoom::ecs::components::TransformComponent>(entity);
  assert(transform_component_opt.has_value()
         && "BodyComponent requires a TransformComponent" != nullptr);
  auto body_def = redoom::physics::BodyDefinition{.type = type_exp.value(),
      .transform = {transform_component_opt.value()},
      .linear_velocity = node["linear_velocity"].as<glm::vec3>(),
      .angular_velocity = node["angular_velocity"].as<float>(),
      .has_fixed_rotation = node["has_fixed_rotation"].as<bool>(),
      .gravity_scale = node["gravity_scale"].as<float>()};
  auto body = scene.getWorld().createBody(body_def);
  auto const fixtures = node["fixtures"];
  if (fixtures) {
    for (auto const& fixture_node : fixtures) {
      deserializeFixture(fixture_node, *body);
    }
  }
  scene.getRegistry().attachComponent<BodyComponent>(
      entity, BodyComponent{body});
  return {};
}
