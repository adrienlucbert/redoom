#include <redoom/ecs/SystemManager.hh>

#include <algorithm>
#include <thread>

namespace redoom::ecs
{
void SystemManager::update(long elapsed_time) noexcept
{
  auto threads = std::vector<std::thread>{};
  threads.reserve(this->systems.size());
  for (auto& system : this->systems)
    threads.emplace_back(std::thread([&]() { system->update(elapsed_time); }));
  for (auto& thread : threads)
    thread.join();
}
} // namespace redoom::ecs
