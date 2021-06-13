#include <benchmark/benchmark.h>

#include <redoom/ecs/Component.hh>
#include <redoom/memory/Allocator.hh>

auto constexpr iterations_count = 2 << 20;

struct DummyComponent : public redoom::ecs::Component<DummyComponent> {
};

using T = DummyComponent;

using redoom::memory::Allocator;

static void BM_Allocation(benchmark::State& state)
{
  auto allocator = Allocator<T>{};
  for (auto _ : state)
    allocator.get();
}
BENCHMARK(BM_Allocation)->Iterations(iterations_count);
