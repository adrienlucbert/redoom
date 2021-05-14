#include <redoom/ecs/Entity.hh>

#include <atomic>

namespace redoom::ecs
{
unsigned int Entity::getUniqueId() noexcept
{
  static std::atomic<unsigned int> id = 0;
  return id++;
}

Entity::Entity() noexcept
  : id{Entity::getUniqueId()}
{
}

unsigned int Entity::getId() const noexcept
{
  return this->id;
}
} // namespace redoom::ecs
