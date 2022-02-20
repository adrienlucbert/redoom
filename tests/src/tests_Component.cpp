#include <catch2/catch.hpp>

#include <thread>

#include <mock/components.hh>

using redoom::ecs::Component;

TEST_CASE("[Component] Basic tests", "[ECS][Component]")
{
  SECTION("Each component type has a unique id")
  {
    CHECK(Component<mock::TestComponent1>::getTypeId()
          != Component<mock::TestComponent2>::getTypeId());
  }
}

TEST_CASE("[Component] Thread safety tests", "[.][Thread][ECS][Component]")
{
  SECTION("A component type id can be requested from different threads")
  {
    auto t1 = std::thread{
        []() { (void)Component<mock::TestComponent1>::getTypeId(); }};
    auto t2 = std::thread{
        []() { (void)Component<mock::TestComponent2>::getTypeId(); }};
    t1.join();
    t2.join();
  }
}
