#include <catch2/catch_test_macros.hpp>

#include <thread>

#include <redoom/ecs/System.hh>
#include <redoom/ecs/SystemManager.hh>

using redoom::ecs::System;
using redoom::ecs::SystemManager;

class DummySystem1 final : public System
{
  void update(long /*elapsed_time*/) noexcept override
  {
  }
};

class DummySystem2 final : public System
{
  void update(long /*elapsed_time*/) noexcept override
  {
  }
};

TEST_CASE("[SystemManager] Basic tests", "[ECS][System]")
{
  auto system_manager = SystemManager{};

  SECTION("?")
  {
    system_manager.make<DummySystem1>();
    system_manager.make<DummySystem2>();
  }
}

TEST_CASE(
    "[SystemManager] Thread safety tests", "[.][Thread][ECS][SystemManager]")
{
  auto system_manager = SystemManager{};

  SECTION("Systems can be created and released from different threads")
  {
    auto t1 = std::thread{[&]() { system_manager.make<DummySystem1>(); }};
    auto t2 = std::thread{[&]() { system_manager.make<DummySystem2>(); }};
    t1.join();
    t2.join();
  }

  SECTION("Systems can be updated from different threads")
  {
    system_manager.make<DummySystem1>();
    system_manager.make<DummySystem2>();
    auto t1 = std::thread{[&]() { system_manager.update(0); }};
    auto t2 = std::thread{[&]() { system_manager.update(0); }};
    t1.join();
    t2.join();
  }
}
