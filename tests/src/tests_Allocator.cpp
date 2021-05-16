#include <catch2/catch_message.hpp>
#include <catch2/catch_test_macros.hpp>

#include <thread>

#include <redoom/memory/Allocator.hh>

using redoom::memory::Allocator;

TEST_CASE("[Allocator] Basic tests", "[Allocator]")
{
  auto int_allocator = Allocator<int>{};

  SECTION("Allocated objects are different")
  {
    auto* i1_p = static_cast<int*>(int_allocator.get(1));
    auto* i2_p = static_cast<int*>(int_allocator.get(2));
    REQUIRE(i1_p != i2_p);
    CHECK(*i1_p == 1);
    CHECK(*i2_p == 2);
    int_allocator.release(i1_p);
    int_allocator.release(i2_p);
  }
}

// TODO(alucbert) uncomment these tests once the Allocator is an Object Pool
// TEST_CASE("[Allocator] Object pool implementation tests", "[Allocator]")
// {
//   auto int_allocator = Allocator<int>{};
//
//   SECTION("Reallocation reuses memory chunks")
//   {
//     auto* i1_p = static_cast<int*>(int_allocator.get(1));
//     auto* i2_p = static_cast<int*>(int_allocator.get(2));
//     int_allocator.release(i1_p);
//     auto* i3_p = static_cast<int*>(int_allocator.get(3));
//     int_allocator.release(i2_p);
//     int_allocator.release(i3_p);
//     CHECK(i1_p == i3_p);
//   }
//
//   SECTION("Objects are contiguous")
//   {
//     auto* i1_p = int_allocator.get(1);
//     auto* i2_p = int_allocator.get(2);
//     auto* i3_p = int_allocator.get(3);
//     CHECK(static_cast<char*>(i2_p) - static_cast<char*>(i1_p) ==
//     sizeof(int));
//   }
// }

TEST_CASE("[Allocator] Thread safety tests", "[.][Thread][Allocator]")
{
  auto int_allocator = Allocator<int>{};

  SECTION("Objects can be allocated and released from different threads")
  {
    auto t1 = std::thread([&]() {
      auto* i1_p = static_cast<int*>(int_allocator.get(1));
      int_allocator.release(i1_p);
    });
    auto t2 = std::thread([&]() {
      auto* i2_p = static_cast<int*>(int_allocator.get(2));
      int_allocator.release(i2_p);
    });
    t1.join();
    t2.join();
  }
}
