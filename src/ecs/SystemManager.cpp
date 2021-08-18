#include <redoom/ecs/SystemManager.hh>

#include <algorithm>
#include <iostream>
#include <thread>

namespace redoom::ecs
{
void SystemManager::update(UpdateContext& context) noexcept
{
  // auto threads = std::vector<std::thread>{};
  // threads.reserve(this->systems.size());
  for (auto& pair : this->systems)
    pair.second.system->update(context);
  // threads.emplace_back(
  //     std::thread([&]() { pair.second.system->update(context); }));
  // for (auto& thread : threads)
  //   thread.join();
}
} // namespace redoom::ecs
