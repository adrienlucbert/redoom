#include <catch2/catch.hpp>

#include <memory>

#include <stringmakers/stringmakers.hpp>

#include <redoom/physics/Body.hh>
#include <redoom/physics/Force.hh>
#include <redoom/physics/World.hh>
#include <redoom/physics/shapes/Cuboid.hh>

using namespace Catch::literals; // NOLINT(google-build-using-namespace)
using redoom::physics::BodyTransform;
using redoom::physics::Cuboid;
using redoom::physics::World;

TEST_CASE("[World] Basic tests", "[Physics][World]")
{
  auto world = World{};
  auto cuboid = std::make_shared<Cuboid>(1.0f, 1.0f, 1.0f);
  auto transform1 = BodyTransform{};
  auto body1 = world.createBody({.transform = transform1});
  body1->createFixture({.shape = cuboid, .density = 1.0f});
  auto transform2 = BodyTransform{{.position = {0.0f, 1.0f, 0.0f}}};
  auto body2 = world.createBody({.transform = transform2});
  body2->createFixture({.shape = cuboid, .density = 1.0f});

  SECTION("Apply global constant forces")
  {
    world.addGlobalConstantForce({.force = {0.0f, -1.0f, 0.0f}});
    world.step(0.5);
    CHECK(transform1.getPosition() == glm::vec3{0.0f, -0.5f, 0.0f});
    CHECK(transform2.getPosition() == glm::vec3{0.0f, 0.5f, 0.0f});
    world.step(1.0);
    CHECK(transform1.getPosition() == glm::vec3{0.0f, -2.0f, 0.0f});
    CHECK(transform2.getPosition() == glm::vec3{0.0f, -1.0f, 0.0f});
  }

  SECTION("Apply body-specific constant forces")
  {
    body2->addConstantForce({.force = {0.0f, -1.0f, 0.0f}});
    world.step(0.5);
    CHECK(transform1.getPosition() == glm::vec3{0.0f, 0.0f, 0.0f});
    CHECK(transform2.getPosition() == glm::vec3{0.0f, 0.5f, 0.0f});
    world.step(1.0);
    CHECK(transform1.getPosition() == glm::vec3{0.0f, 0.0f, 0.0f});
    CHECK(transform2.getPosition() == glm::vec3{0.0f, -1.0f, 0.0f});
  }

  SECTION("Apply body-specific non-constant forces")
  {
    body2->addForce({.force = {0.0f, -1.0f, 0.0f}});
    world.step(0.5);
    CHECK(transform1.getPosition() == glm::vec3{0.0f, 0.0f, 0.0f});
    CHECK(transform2.getPosition() == glm::vec3{0.0f, 0.5f, 0.0f});
    world.step(0.5);
    CHECK(transform1.getPosition() == glm::vec3{0.0f, 0.0f, 0.0f});
    CHECK(transform2.getPosition() == glm::vec3{0.0f, 0.0f, 0.0f});
    body2->addForce({.force = {0.0f, -1.0f, 0.0f}});
    world.step(1.0);
    CHECK(transform1.getPosition() == glm::vec3{0.0f, 0.0f, 0.0f});
    CHECK(transform2.getPosition() == glm::vec3{0.0f, -1.5f, 0.0f});
  }
}
