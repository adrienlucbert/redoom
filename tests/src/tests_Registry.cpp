#include <catch2/catch_test_macros.hpp>

#include <redoom/ecs/Component.hh>
#include <redoom/ecs/ComponentManager.hh>
#include <redoom/ecs/Registry.hh>
#include <redoom/ecs/System.hh>

#include <thread>

using redoom::ecs::Component;
using redoom::ecs::Registry;
using redoom::ecs::System;

class DummyComponent1 final : public Component<DummyComponent1>
{
};

class DummyComponent2 final : public Component<DummyComponent2>
{
};

class DummySystem1 final : public System<DummySystem1>
{
};

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
    registry.releaseEntity(entity);
    // TODO(alucbert): add checks
  }

  SECTION("Components can be attached to entities")
  {
    auto& entity = registry.makeEntity();
    registry.attachComponent<DummyComponent1>(entity);
    // TODO(alucbert): check if entity has component
  }

  SECTION("Components can be detached from entities")
  {
    auto& entity = registry.makeEntity();
    registry.attachComponent<DummyComponent1>(entity);
    // TODO(alucbert): check if entity has component
    registry.detachComponent<DummyComponent1>(entity);
    // TODO(alucbert): check if entity no longer has component
  }

  SECTION("Systems can be attached to a registry")
  {
    registry.attachSystem<DummySystem1>();
    // TODO(alucbert): check if system is attached
  }

  SECTION("Systems can be detached from a registry")
  {
    registry.attachSystem<DummySystem1>();
    // TODO(alucbert): check if system is attached
    registry.detachSystem<DummySystem1>();
    // TODO(alucbert): check if system is no longer attached
  }

  SECTION("Systems can be updated")
  {
    registry.attachSystem<DummySystem1>();
    registry.update(0);
    // TODO(alucbert): add checks
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
      registry.attachSystem<DummySystem1>();
      registry.detachSystem<DummySystem1>();
    });
    auto t2 = std::thread([&]() {
      registry.attachSystem<DummySystem1>();
      registry.detachSystem<DummySystem1>();
    });
    t1.join();
    t2.join();
  }
}
