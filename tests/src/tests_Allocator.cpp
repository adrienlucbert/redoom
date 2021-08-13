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
    int* i2_p = nullptr;
    int* i3_p = nullptr;
    {
      // NOTE: Create a chunk of size 1
      auto i1 = int_allocator.get(1);
      i1_p = i1.get();
      // NOTE: Create a chunk of size 2
      // The object pool searches for the first non-empty chunk from the last to
      // the first. When allocating i4, the slot of i2 will be reused, when
      // allocating i5, the slot of i3 will be reused, and when allocating i6,
      // the place of i1 will be reused.
      auto i2 = int_allocator.get(2);
      i2_p = i2.get();
      auto i3 = int_allocator.get(3);
      i3_p = i3.get();
    }
    auto i4 = int_allocator.get(4);
    CHECK(i2_p == i4.get());
    auto i5 = int_allocator.get(5);
    CHECK(i3_p == i5.get());
    auto i6 = int_allocator.get(6);
    CHECK(i1_p == i6.get());
  }

  SECTION("Objects are contiguous")
  {
    // NOTE: Create a chunk of size 1
    auto i1 = int_allocator.get(1);
    // NOTE: Create a chunk of size 2, and test contiguousness on this chunk
    auto i2 = int_allocator.get(2);
    auto i3 = int_allocator.get(3);
    // NOLINTNEXTLINE(cppcoreguidelines-pro-type-reinterpret-cast)
    auto diff = reinterpret_cast<std::intptr_t>(i3.get())
              // NOLINTNEXTLINE(cppcoreguidelines-pro-type-reinterpret-cast)
              - reinterpret_cast<std::intptr_t>(i2.get());
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
