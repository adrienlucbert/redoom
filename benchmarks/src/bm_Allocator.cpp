#include <benchmark/benchmark.h>

#include <chrono>
#include <iostream>

#include <redoom/ecs/Component.hh>
#include <redoom/memory/Allocator.hh>

auto constexpr iterations_count = 2000000;
auto constexpr traversals_count = 300;

struct DummyComponent : public redoom::ecs::Component<DummyComponent> {
  [[nodiscard]] std::string const& getType() const noexcept override
  {
    static auto const type = std::string{"DummyComponent"};
    return type;
  }
};

using T = DummyComponent;

using redoom::memory::Allocator;

static void BM_Allocation(benchmark::State& state)
{
  auto allocator = Allocator<T>{};
  auto start = std::chrono::high_resolution_clock::now();
  {
    auto ptrs = std::vector<Allocator<T>::ptr_t>{}; // keep ptrs to avoid
                                                    // destroying them on the go
    ptrs.reserve(state.iterations());
    auto index = 0u;
    auto start = std::chrono::high_resolution_clock::now();
    for (auto _ : state)
      ptrs.emplace_back(allocator.get());
    auto end = std::chrono::high_resolution_clock::now();
    auto duration =
        std::chrono::duration_cast<std::chrono::nanoseconds>(end - start);
    std::cout << "Allocation: " << duration.count() / iterations_count << "ns/u"
              << '\n';
    start = end;
    for (auto i = 0u; i < traversals_count; ++i)
      for (auto const& ptr : ptrs)
        ;
    end = std::chrono::high_resolution_clock::now();
    duration =
        std::chrono::duration_cast<std::chrono::nanoseconds>(end - start);
    std::cout << "Traversal: "
              << duration.count() / traversals_count / ptrs.size() << "ns/u"
              << '\n';
    start = end;
  }
  auto end = std::chrono::high_resolution_clock::now();
  auto duration =
      std::chrono::duration_cast<std::chrono::nanoseconds>(end - start);
  std::cout << "Deallocation: " << duration.count() / iterations_count << "ns/u"
            << '\n';
}
BENCHMARK(BM_Allocation)->Iterations(iterations_count);
