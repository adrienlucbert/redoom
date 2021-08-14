#include <redoom/ecs/SystemManager.hh>

#include <algorithm>
#include <thread>

namespace redoom::ecs
{
void SystemManager::update(UpdateContext& context) noexcept
{
  auto threads = std::vector<std::thread>{};
  threads.reserve(this->systems.size());
  for (auto& pair : this->systems)
    threads.emplace_back(std::thread([&]() { pair.second->update(context); }));
  for (auto& thread : threads)
    thread.join();
}
} // namespace redoom::ecs
