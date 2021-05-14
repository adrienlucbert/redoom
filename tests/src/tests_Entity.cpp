#include <catch2/catch_test_macros.hpp>

#include <redoom/ecs/Entity.hh>
#include <thread>

using redoom::ecs::Entity;

TEST_CASE("[Entity] Basic tests", "[ECS][Entity]")
{
  SECTION("Different id for each Entity instance")
  {
    CHECK(Entity{}.getId() != Entity{}.getId());
  }
}

TEST_CASE("[Entity] Thread safety tests", "[.][Thread][ECS][Entity]")
{
  SECTION("Different id for each Entity instance")
  {
    auto t1 = std::thread{[]() { (void)Entity{}.getId(); }};
    auto t2 = std::thread{[]() { (void)Entity{}.getId(); }};
    t1.join();
    t2.join();
  }
}
