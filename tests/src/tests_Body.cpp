#include <catch2/catch.hpp>

#include <memory>

#include <redoom/physics/Body.hh>
#include <redoom/physics/World.hh>
#include <redoom/physics/shapes/Cuboid.hh>
#include <redoom/physics/shapes/Sphere.hh>

using namespace Catch::literals; // NOLINT(google-build-using-namespace)
using redoom::physics::AABB;
using redoom::physics::BodyTransform;
using redoom::physics::Cuboid;
using redoom::physics::World;

TEST_CASE("[Body] Basic tests", "[Physics][Body]")
{
  auto world = World{};

  SECTION("Create body from definition")
  {
    auto transform = BodyTransform{{.position = glm::vec3{1.0f}}};
    auto body = world.createBody({.transform = transform});
    CHECK(std::addressof(body->getWorld()) == std::addressof(world));
  }

  SECTION("Body AABB is calculated from all fixtures")
  {
    auto transform = BodyTransform{{.position = glm::vec3{1.0f}}};
    auto body = world.createBody({.transform = transform});
    CHECK(!body->getLocalAABB().has_value());
    CHECK(!body->getGlobalAABB().has_value());
    auto cuboid = std::make_shared<Cuboid>(1.0f, 1.0f, 1.0f);
    body->createFixture({.shape = cuboid});
    CHECK(body->getLocalAABB() == AABB{{-.5f, -.5f, -.5f}, {.5f, .5f, .5f}});
    CHECK(body->getGlobalAABB() == AABB{{.5f, .5f, .5f}, {1.5f, 1.5f, 1.5f}});
    body->createFixture({.shape = cuboid, .local_position = glm::vec3{1.0f}});
    CHECK(body->getLocalAABB() == AABB{{-.5f, -.5f, -.5f}, {1.5f, 1.5f, 1.5f}});
    CHECK(body->getGlobalAABB() == AABB{{.5f, .5f, .5f}, {2.5f, 2.5f, 2.5f}});
  }

  SECTION("Body mass is calculated from all fixtures")
  {
    auto transform = BodyTransform{{.position = glm::vec3{1.0f}}};
    auto body = world.createBody({.transform = transform});
    auto cuboid = std::make_shared<Cuboid>(1.0f, 1.0f, 1.0f);
    CHECK(body->getMass() == 0.0_a);
    body->createFixture({.shape = cuboid, .density = 1.0f});
    CHECK(body->getMass() == 1.0_a);
    body->createFixture({.shape = cuboid, .density = 5.0f});
    CHECK(body->getMass() == 6.0_a);
  }
}
