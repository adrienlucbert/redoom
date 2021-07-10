#include <catch2/catch_test_macros.hpp>

#include <cstddef>
#include <cstdint>
#include <thread>

#include <redoom/memory/Allocator.hh>

using redoom::memory::Allocator;

TEST_CASE("[Allocator] Basic tests", "[Allocator]")
{
  auto int_allocator = Allocator<int>{};

  SECTION("Allocated objects are different")
  {
    auto i1 = int_allocator.get(1);
    auto i2 = int_allocator.get(2);
    REQUIRE(i1 != i2);
    CHECK(*i1 == 1);
    CHECK(*i2 == 2);
  }
}

TEST_CASE("[Allocator] Object pool implementation tests", "[Allocator]")
{
  auto int_allocator = Allocator<int>{};

  SECTION("Reallocation reuses memory chunks")
  {
    int* i1_p = nullptr;
    {
      auto i1 = int_allocator.get(1);
      i1_p = i1.get();
      auto i2 = int_allocator.get(2);
    }
    auto i3 = int_allocator.get(3);
    CHECK(i1_p == i3.get());
  }

  SECTION("Objects are contiguous")
  {
    auto i1 = int_allocator.get(1);
    auto i2 = int_allocator.get(2);
    // NOLINTNEXTLINE(cppcoreguidelines-pro-type-reinterpret-cast)
    auto diff = reinterpret_cast<std::intptr_t>(i2.get())
              // NOLINTNEXTLINE(cppcoreguidelines-pro-type-reinterpret-cast)
              - reinterpret_cast<std::intptr_t>(i1.get());
    CHECK(diff == sizeof(int));
  }
}

TEST_CASE("[Allocator] Thread safety tests", "[.][Thread][Allocator]")
{
  auto int_allocator = Allocator<int>{};

  SECTION("Objects can be allocated and released from different threads")
  {
    auto t1 = std::thread([&]() { int_allocator.get(1); });
    auto t2 = std::thread([&]() { int_allocator.get(2); });
    t1.join();
    t2.join();
  }
}
