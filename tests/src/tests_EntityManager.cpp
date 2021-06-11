#include <catch2/catch_test_macros.hpp>

#include <thread>

#include <redoom/ecs/EntityManager.hh>

using redoom::ecs::EntityManager;

TEST_CASE("[EntityManager] Basic tests", "[ECS][Entity]")
{
  auto entity_manager = EntityManager{};

  SECTION("Each entity instance has a unique id")
  {
    auto entity1 = entity_manager.make();
    auto entity2 = entity_manager.make();
    CHECK(entity1.getId() != entity2.getId());
  }
}

TEST_CASE(
    "[EntityManager] Thread safety tests", "[.][Thread][ECS][EntityManager]")
{
  auto entity_manager = EntityManager{};

  SECTION("Entities can be created and released from different threads")
  {
    auto t1 = std::thread{[&]() { auto entity = entity_manager.make(); }};
    auto t2 = std::thread{[&]() { auto entity = entity_manager.make(); }};
    t1.join();
    t2.join();
  }
}
