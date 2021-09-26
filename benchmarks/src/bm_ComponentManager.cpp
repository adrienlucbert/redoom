#include <benchmark/benchmark.h>

#include <redoom/ecs/Component.hh>
#include <redoom/ecs/ComponentManager.hh>
#include <redoom/ecs/Entity.hh>

auto constexpr iterations_count = 2 << 12;

struct DummyComponent : public redoom::ecs::Component<DummyComponent> {
  [[nodiscard]] std::string const& getType() const noexcept override
  {
    static auto const type = std::string{"DummyComponent"};
    return type;
  }
};

using T = DummyComponent;

using redoom::ecs::ComponentManager;
using redoom::ecs::Entity;

static void BM_Iterating(benchmark::State& state)
{
  auto component_manager = ComponentManager{};
  for (auto i = 0u; i < 2 << 10; ++i)
    (void)component_manager.make<T>(i);
  for (auto _ : state)
    component_manager.apply<DummyComponent>(
        [](Entity /*unused*/, DummyComponent& /*unused*/) {});
}
BENCHMARK(BM_Iterating)->Iterations(iterations_count);
