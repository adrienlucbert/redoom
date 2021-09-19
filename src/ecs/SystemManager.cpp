#include <redoom/ecs/SystemManager.hh>

#include <vector>

namespace redoom::ecs
{
void SystemManager::update(UpdateContext& context) noexcept
{
  auto futures = std::vector<std::future<void>>{};

  for (auto& pair : this->systems) {
    auto& system = pair.second.system;
    if (system->is_multithreaded) {
      auto future = this->thread_pool.enqueue([&] { system->update(context); });
      futures.emplace_back(std::move(future));
    } else {
      system->update(context);
    }
  }

  for (auto& future : futures) {
    future.wait();
  }
}
} // namespace redoom::ecs
