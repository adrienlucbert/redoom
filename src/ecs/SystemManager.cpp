#include <redoom/ecs/SystemManager.hh>

#include <algorithm>
#include <thread>

namespace redoom::ecs
{
void SystemManager::update(
    long elapsed_time, ComponentManager& component_manager) noexcept
{
  auto threads = std::vector<std::thread>{};
  threads.reserve(this->systems.size());
  for (auto& pair : this->systems)
    threads.emplace_back(std::thread([&]() {
      // Call every overload of the `update` method.
      // Only 1 should be implemented, the others are SystemBase'
      // implemetations, which have no effect.
      pair.second->update(elapsed_time);
      pair.second->update(elapsed_time, component_manager);
    }));
  for (auto& thread : threads)
    thread.join();
}
} // namespace redoom::ecs
