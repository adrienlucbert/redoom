#include <catch2/catch.hpp>

#include <thread>

#include <mock/Window.hh>
#include <mock/systems.hh>
#include <redoom/ecs/ComponentManager.hh>
#include <redoom/ecs/EntityManager.hh>
#include <redoom/ecs/SystemManager.hh>

using redoom::ecs::ComponentManager;
using redoom::ecs::EntityManager;
using redoom::ecs::SystemManager;
using redoom::ecs::UpdateContext;

TEST_CASE("[SystemManager] Basic tests", "[ECS][System]")
{
  auto system_manager = SystemManager{};

  SECTION("Systems can be created using the SystemManager")
  {
    system_manager.make<mock::TestSystem1>();
    system_manager.make<mock::TestSystem2>();
  }
}

TEST_CASE(
    "[SystemManager] Thread safety tests", "[.][Thread][ECS][SystemManager]")
{
  auto system_manager = SystemManager{};
  auto component_manager = ComponentManager{};
  auto entity_manager = EntityManager{};
  auto window = mock::TestWindow{};
  auto context = UpdateContext{component_manager, entity_manager, 0, window};

  SECTION("Systems can be created and released from different threads")
  {
    auto t1 = std::thread{[&]() { system_manager.make<mock::TestSystem1>(); }};
    auto t2 = std::thread{[&]() { system_manager.make<mock::TestSystem2>(); }};
    t1.join();
    t2.join();
  }
}
