#include <redoom/Scene.hh>

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
} // namespace redoom
