#include <redoom/Scene.hh>

#include <iostream>

#include <redoom/serializer/SceneSerializer.hh>

namespace redoom
{
Scene::Scene(std::string_view pname) noexcept
  : name{pname}
{
}

std::string const& Scene::getName() const noexcept
{
  return this->name;
}

ecs::Registry& Scene::getRegistry() noexcept
{
  return this->registry;
}

ecs::Registry const& Scene::getRegistry() const noexcept
{
  return this->registry;
}

physics::World& Scene::getWorld() noexcept
{
  return this->world;
}

physics::World const& Scene::getWorld() const noexcept
{
  return this->world;
}

void Scene::serialize(std::string_view filepath) const noexcept
{
  auto exp = SceneSerializer::get().serialize(filepath, *this);
  if (!exp)
    std::cerr << exp.error() << '\n';
}

void Scene::setName(std::string_view pname) noexcept
{
  this->name = pname;
}
} // namespace redoom
