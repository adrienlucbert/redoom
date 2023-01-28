#include <catch2/catch.hpp>

#include <thread>

#include <mock/Window.hh>
#include <mock/components.hh>
#include <mock/systems.hh>
#include <redoom/ecs/Component.hh>
#include <redoom/ecs/ComponentManager.hh>
#include <redoom/ecs/Entity.hh>
#include <redoom/ecs/Registry.hh>
#include <redoom/ecs/System.hh>

using redoom::ecs::Entity;
using redoom::ecs::Registry;

TEST_CASE("[Registry] Basic tests", "[ECS][Registry]")
{
  auto registry = Registry{};

  SECTION("Entities can be created from a registry")
  {
    auto entity1 = registry.makeEntity();
    auto entity2 = registry.makeEntity();
    CHECK(entity2 != entity1);
  }

  SECTION("Entities can be released from a registry")
  {
    auto entity = registry.makeEntity();
    CHECK(registry.hasEntity(entity));
    registry.releaseEntity(entity);
  }

  SECTION("Components can be attached to entities")
  {
    auto entity = registry.makeEntity();
    registry.attachComponent<mock::TestComponent1>(entity);
    CHECK(registry.hasComponent<mock::TestComponent1>(entity));
  }

  SECTION("Components can be detached from entities")
  {
    auto entity = registry.makeEntity();
    registry.attachComponent<mock::TestComponent1>(entity);
    CHECK(registry.hasComponent<mock::TestComponent1>(entity));
    registry.detachComponent<mock::TestComponent1>(entity);
    CHECK(!registry.hasComponent<mock::TestComponent1>(entity));
  }

  SECTION("Systems can be attached to a registry")
  {
    registry.attachSystem<mock::TestSystem1>();
    CHECK(registry.hasSystem<mock::TestSystem1>());
  }

  SECTION("Systems can be detached from a registry")
  {
    registry.attachSystem<mock::TestSystem1>();
    REQUIRE(registry.hasSystem<mock::TestSystem1>());
    registry.detachSystem<mock::TestSystem1>();
    CHECK(!registry.hasSystem<mock::TestSystem1>());
  }

  SECTION("Systems can be updated")
  {
    auto window = mock::TestWindow{};
    mock::TestSystem1::isUpdated = false;
    registry.attachSystem<mock::TestSystem1>();
    REQUIRE(mock::TestSystem1::isUpdated == false);
    registry.update(window, 14.0);
    REQUIRE(mock::TestSystem1::isUpdated == true);
  }
}

TEST_CASE("[Registry] Thread safety tests", "[.][Thread][ECS][Registry]")
{
  auto registry = Registry{};

  SECTION("Entities can be created and released from different threads")
  {
    auto t1 = std::thread([&]() {
      auto entity = registry.makeEntity();
      registry.releaseEntity(entity);
    });
    auto t2 = std::thread([&]() {
      auto entity = registry.makeEntity();
      registry.releaseEntity(entity);
    });
    t1.join();
    t2.join();
  }

  SECTION("Components can be attached and detached from different threads")
  {
    auto entity = registry.makeEntity();
    auto t1 = std::thread([&]() {
      registry.attachComponent<mock::TestComponent1>(entity);
      registry.detachComponent<mock::TestComponent1>(entity);
    });
    auto t2 = std::thread([&]() {
      registry.attachComponent<mock::TestComponent2>(entity);
      registry.detachComponent<mock::TestComponent2>(entity);
    });
    t1.join();
    t2.join();
  }

  SECTION("Systems can be attached and detached from different threads")
  {
    auto t1 = std::thread([&]() {
      registry.attachSystem<mock::TestSystem1>();
      registry.detachSystem<mock::TestSystem1>();
    });
    auto t2 = std::thread([&]() {
      registry.attachSystem<mock::TestSystem2>();
      registry.detachSystem<mock::TestSystem2>();
    });
    t1.join();
    t2.join();
  }
}
