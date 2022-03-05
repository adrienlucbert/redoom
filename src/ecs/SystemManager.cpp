#include <redoom/ecs/SystemManager.hh>

#include <vector>

namespace redoom::ecs
{
void SystemManager::update(UpdateContext& context) noexcept
{
  auto futures = std::vector<std::future<void>>{};

  for (auto& pair : this->systems_) {
    auto& system = pair.second.system;
    if (system->isMultithreaded()) {
      auto future =
          this->thread_pool_.enqueue([&] { system->update(context); });
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
