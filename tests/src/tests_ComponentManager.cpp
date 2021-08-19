#include <catch2/catch_test_macros.hpp>

#include <thread>

#include <mock/components.hh>
#include <redoom/ecs/ComponentManager.hh>

using redoom::ecs::ComponentManager;

TEST_CASE("[ComponentManager] Basic tests", "[ECS][Component]")
{
  auto component_manager = ComponentManager{};

  SECTION("Each component instance has a unique id")
  {
    auto& component1 = component_manager.make<mock::TestComponent1>(0);
    auto& component2 = component_manager.make<mock::TestComponent2>(0);
    CHECK(component1.getTypeId() != component2.getTypeId());
    component_manager.release<mock::TestComponent1>(0);
    component_manager.release<mock::TestComponent2>(0);
  }
}

TEST_CASE("[ComponentManager] Thread safety tests",
    "[.][Thread][ECS][ComponentManager]")
{
  auto component_manager = ComponentManager{};

  SECTION("Components can be created and released from different threads")
  {
    auto t1 = std::thread([&]() {
      auto& component = component_manager.make<mock::TestComponent1>(0);
      component_manager.release<mock::TestComponent1>(0);
    });
    auto t2 = std::thread([&]() {
      auto& component = component_manager.make<mock::TestComponent2>(0);
      component_manager.release<mock::TestComponent2>(0);
    });
    t1.join();
    t2.join();
  }
}
