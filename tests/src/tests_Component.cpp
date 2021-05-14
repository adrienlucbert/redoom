#include <catch2/catch_test_macros.hpp>

#include <redoom/ecs/Component.hh>
#include <thread>

using redoom::ecs::Component;

class DummyComponent1 final : public Component<DummyComponent1>
{
};

class DummyComponent2 final : public Component<DummyComponent2>
{
};

TEST_CASE("[Component] Basic tests", "[ECS][Component]")
{
  SECTION("Different type id for each component type")
  {
    CHECK(Component<DummyComponent1>::getTypeId()
          != Component<DummyComponent2>::getTypeId());
  }
}

TEST_CASE("[Component] Thread safety tests", "[.][Thread][ECS][Component]")
{
  SECTION("Different type id for each component type")
  {
    auto t1 =
        std::thread{[]() { (void)Component<DummyComponent1>::getTypeId(); }};
    auto t2 =
        std::thread{[]() { (void)Component<DummyComponent2>::getTypeId(); }};
    t1.join();
    t2.join();
  }
}
