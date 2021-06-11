#include <catch2/catch_test_macros.hpp>

#include <thread>

#include <redoom/ecs/Component.hh>
#include <redoom/ecs/ComponentManager.hh>

using redoom::ecs::Component;
using redoom::ecs::ComponentManager;

class DummyComponent1 final : public Component<DummyComponent1>
{
};

class DummyComponent2 final : public Component<DummyComponent2>
{
};

TEST_CASE("[ComponentManager] Basic tests", "[ECS][Component]")
{
  auto component_manager = ComponentManager{};

  SECTION("Each component instance has a unique id")
  {
    auto& component1 = component_manager.make<DummyComponent1>(0);
    auto& component2 = component_manager.make<DummyComponent2>(0);
    CHECK(component1.getTypeId() != component2.getTypeId());
    component_manager.release<DummyComponent1>(component1);
    component_manager.release<DummyComponent2>(component2);
  }
}

TEST_CASE("[ComponentManager] Thread safety tests",
    "[.][Thread][ECS][ComponentManager]")
{
  auto component_manager = ComponentManager{};

  SECTION("Components can be created and released from different threads")
  {
    auto t1 = std::thread([&]() {
      auto& component = component_manager.make<DummyComponent1>(0);
      component_manager.release<DummyComponent1>(component);
    });
    auto t2 = std::thread([&]() {
      auto& component = component_manager.make<DummyComponent2>(0);
      component_manager.release<DummyComponent2>(component);
    });
    t1.join();
    t2.join();
  }
}
