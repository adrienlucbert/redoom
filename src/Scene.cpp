#include <redoom/Scene.hh>

#include <iostream>

#include <redoom/serializer/SceneSerializer.hh>

namespace redoom
{
Scene::Scene(std::string_view name) noexcept
  : name_{name}
{
}

std::string const& Scene::getName() const noexcept
{
  return this->name_;
}

ecs::Registry& Scene::getRegistry() noexcept
{
  return this->registry_;
}

ecs::Registry const& Scene::getRegistry() const noexcept
{
  return this->registry_;
}

physics::World& Scene::getWorld() noexcept
{
  return this->world_;
}

physics::World const& Scene::getWorld() const noexcept
{
  return this->world_;
}

void Scene::serialize(std::string_view filepath) const noexcept
{
  auto exp = SceneSerializer::get().serialize(filepath, *this);
  if (!exp)
    std::cerr << exp.error() << '\n';
}

void Scene::setName(std::string_view name) noexcept
{
  this->name_ = name;
}
} // namespace redoom
