#include <catch2/catch_test_macros.hpp>

#include <thread>

#include <redoom/ecs/Component.hh>
#include <redoom/ecs/ComponentManager.hh>
#include <redoom/ecs/Registry.hh>
#include <redoom/ecs/System.hh>

using redoom::ecs::Component;
using redoom::ecs::Registry;
using redoom::ecs::System;

namespace
{
class DummyComponent1 final : public Component<DummyComponent1>
{
};

class DummyComponent2 final : public Component<DummyComponent2>
{
};

// This global variable is used to test DummySystem's execution
auto test_value = 0L; // NOLINT

class DummySystem : public System<DummySystem> // NOLINT
{
public:
  void update(long elapsed_time) noexcept override
  {
    test_value = elapsed_time;
  }
};
} // namespace

TEST_CASE("[Registry] Basic tests", "[ECS][Registry]")
{
  auto registry = Registry{};

  SECTION("Entities can be created from a registry")
  {
    auto& entity1 = registry.makeEntity();
    auto& entity2 = registry.makeEntity();
    CHECK(entity2.getId() > entity1.getId());
  }

  SECTION("Entities can be released from a registry")
  {
    auto& entity = registry.makeEntity();
    CHECK(registry.hasEntity(entity));
    registry.releaseEntity(entity);
  }

  SECTION("Components can be attached to entities")
  {
    auto& entity = registry.makeEntity();
    registry.attachComponent<DummyComponent1>(entity);
    CHECK(registry.hasComponent<DummyComponent1>(entity));
  }

  SECTION("Components can be detached from entities")
  {
    auto& entity = registry.makeEntity();
    registry.attachComponent<DummyComponent1>(entity);
    CHECK(registry.hasComponent<DummyComponent1>(entity));
    registry.detachComponent<DummyComponent1>(entity);
    CHECK(!registry.hasComponent<DummyComponent1>(entity));
  }

  SECTION("Systems can be attached to a registry")
  {
    registry.attachSystem<DummySystem>();
    CHECK(registry.hasSystem<DummySystem>());
  }

  SECTION("Systems can be detached from a registry")
  {
    registry.attachSystem<DummySystem>();
    REQUIRE(registry.hasSystem<DummySystem>());
    registry.detachSystem<DummySystem>();
    CHECK(!registry.hasSystem<DummySystem>());
  }

  SECTION("Systems can be updated")
  {
    registry.attachSystem<DummySystem>();
    REQUIRE(test_value == 0);
    registry.update(14);
    REQUIRE(test_value == 14);
  }
}

TEST_CASE("[Registry] Thread safety tests", "[.][Thread][ECS][Registry]")
{
  auto registry = Registry{};

  SECTION("Entities can be created and released from different threads")
  {
    auto t1 = std::thread([&]() {
      auto& entity = registry.makeEntity();
      registry.releaseEntity(entity);
    });
    auto t2 = std::thread([&]() {
      auto& entity = registry.makeEntity();
      registry.releaseEntity(entity);
    });
    t1.join();
    t2.join();
  }

  SECTION("Components can be attached and detached from different threads")
  {
    auto& entity = registry.makeEntity();
    auto t1 = std::thread([&]() {
      registry.attachComponent<DummyComponent1>(entity);
      registry.detachComponent<DummyComponent1>(entity);
    });
    auto t2 = std::thread([&]() {
      registry.attachComponent<DummyComponent2>(entity);
      registry.detachComponent<DummyComponent2>(entity);
    });
    t1.join();
    t2.join();
  }

  SECTION("Systems can be attached and detached from different threads")
  {
    auto t1 = std::thread([&]() {
      registry.attachSystem<DummySystem>();
      registry.detachSystem<DummySystem>();
    });
    auto t2 = std::thread([&]() {
      registry.attachSystem<DummySystem>();
      registry.detachSystem<DummySystem>();
    });
    t1.join();
    t2.join();
  }
}
