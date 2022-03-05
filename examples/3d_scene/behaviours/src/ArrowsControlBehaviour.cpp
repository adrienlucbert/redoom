#include <behaviours/extern.hh>

#include <redoom/Application.hh>
#include <redoom/ecs/Behaviour.hh>
#include <redoom/ecs/components/TransformComponent.hh>
#include <redoom/events/Key.hh>

using redoom::ecs::Behaviour;
using redoom::ecs::Context;
using redoom::ecs::Entity;
using redoom::ecs::UpdateContext;
using redoom::ecs::components::TransformComponent;
using redoom::events::Key;

struct ArrowsControlBehaviour : public Behaviour {
  [[nodiscard]] std::string const& getType() const noexcept override
  {
    static auto const type = std::string{"ArrowsControlBehaviour"};
    return type;
  }

  void onInit(Entity entity, Context& context) noexcept override
  {
    auto opt = context.getComponentManager().get<TransformComponent>(entity);
    if (!opt)
      assert("No transform component found" == nullptr);
    this->transform_ = std::addressof(*opt);
  }

  void onUpdate(Entity /*entity*/, UpdateContext& /*context*/) noexcept override
  {
    auto pos = this->transform_->getPosition();
    auto const speed = 1.0f;

    if (isKeyPressed(Key::UP))
      pos.z += speed;
    if (isKeyPressed(Key::DOWN))
      pos.z -= speed;
    if (isKeyPressed(Key::RIGHT))
      pos.x += speed;
    if (isKeyPressed(Key::LEFT))
      pos.x -= speed;
    if (isKeyPressed(Key::J))
      pos.y += speed;
    if (isKeyPressed(Key::K))
      pos.y -= speed;
    // TODO(alucbert): do not update if not moved
    this->transform_->setPosition(pos);
  }

private:
  TransformComponent* transform_{nullptr};
};

std::unique_ptr<Behaviour> make() noexcept
{
  return std::make_unique<ArrowsControlBehaviour>();
}
