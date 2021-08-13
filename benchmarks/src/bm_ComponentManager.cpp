#include <benchmark/benchmark.h>

#include <redoom/ecs/Component.hh>
#include <redoom/ecs/ComponentManager.hh>

auto constexpr iterations_count = 2 << 12;

struct DummyComponent : public redoom::ecs::Component<DummyComponent> {
};

using T = DummyComponent;

using redoom::ecs::ComponentManager;

static void BM_Iterating(benchmark::State& state)
{
  auto component_manager = ComponentManager{};
  for (auto i = 0u; i < 2 << 10; ++i)
    (void)component_manager.make<T>(i);
  for (auto _ : state)
    component_manager.apply<DummyComponent>([](DummyComponent& /*unused*/) {});
}
BENCHMARK(BM_Iterating)->Iterations(iterations_count);
