#include <redoom/ecs/SystemManager.hh>

#include <vector>

namespace redoom::ecs
{
void SystemManager::runUpdateHook(
    void (SystemBase::*hook)(UpdateContext&), UpdateContext& context) noexcept
{
  auto futures = std::vector<std::future<void>>{};

  for (auto& pair : this->systems_) {
    auto& system = pair.second.system;
    if (system->isMultithreaded()) {
      auto future =
          this->thread_pool_.enqueue([&] { (*system.*hook)(context); });
      futures.emplace_back(std::move(future));
    } else {
      (*system.*hook)(context);
    }
  }

  for (auto& future : futures) {
    future.wait();
  }
}

void SystemManager::update(UpdateContext& context) noexcept
{
  this->runUpdateHook(&SystemBase::beforeUpdate, context);
  this->runUpdateHook(&SystemBase::update, context);
  this->runUpdateHook(&SystemBase::afterUpdate, context);
}
} // namespace redoom::ecs
