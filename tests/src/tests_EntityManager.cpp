#include <catch2/catch_test_macros.hpp>

#include <thread>

#include <redoom/ecs/EntityManager.hh>

using redoom::ecs::EntityManager;

TEST_CASE("[EntityManager] Basic tests", "[ECS][Entity]")
{
  auto entity_manager = EntityManager{};

  SECTION("Different id for each Entity instance")
  {
    auto entity1 = entity_manager.makeEntity();
    auto entity2 = entity_manager.makeEntity();
    CHECK(entity1.getId() != entity2.getId());
    entity_manager.releaseEntity(entity1);
    entity_manager.releaseEntity(entity2);
  }
}

TEST_CASE(
    "[EntityManager] Thread safety tests", "[.][Thread][ECS][EntityManager]")
{
  auto entity_manager = EntityManager{};

  SECTION("Different id for each Entity instance")
  {
    auto t1 = std::thread{[&]() {
      auto entity = entity_manager.makeEntity();
      entity_manager.releaseEntity(entity);
    }};
    auto t2 = std::thread{[&]() {
      auto entity = entity_manager.makeEntity();
      entity_manager.releaseEntity(entity);
    }};
    t1.join();
    t2.join();
  }
}
