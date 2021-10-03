#include <catch2/catch_test_macros.hpp>

#include <atomic>
#include <vector>

#include <Utils/ThreadPool.hh>

TEST_CASE("[ThreadPool] Basic tests", "[Utils][ThreadPool]")
{
  auto tp = redoom::Utils::ThreadPool{};

  SECTION("ThreadPool can reserve threads")
  {
    tp.reserve(5);
    CHECK(tp.size() == 5);
  }

  SECTION("ThreadPool can enqueue tasks until capacity")
  {
    auto counter = std::atomic{0};
    tp.reserve(3);
    auto futures = std::vector<std::future<void>>{};
    for (auto i = 0u; i < 3u; ++i) {
      auto future = tp.enqueue([&]() { ++counter; });
      futures.emplace_back(std::move(future));
    }
    for (auto& future : futures) {
      future.wait();
    }
    CHECK(counter == 3);
  }

  SECTION("ThreadPool can enqueue tasks over capacity")
  {
    auto counter = std::atomic{0};
    tp.reserve(3);
    auto futures = std::vector<std::future<void>>{};
    for (auto i = 0u; i < 10u; ++i) {
      auto future = tp.enqueue([&]() { ++counter; });
      futures.emplace_back(std::move(future));
    }
    for (auto& future : futures) {
      future.wait();
    }
    CHECK(counter == 10);
  }
}
