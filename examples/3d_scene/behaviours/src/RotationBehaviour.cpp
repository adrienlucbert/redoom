#include <behaviours/extern.hh>

#include <redoom/ecs/Behaviour.hh>
#include <redoom/ecs/components/TransformComponent.hh>

using redoom::ecs::Behaviour;
using redoom::ecs::Context;
using redoom::ecs::Entity;
using redoom::ecs::UpdateContext;
using redoom::ecs::components::TransformComponent;

struct RotationBehaviour : public Behaviour {
  [[nodiscard]] std::string const& getType() const noexcept override
  {
    static auto const type = std::string{"RotationBehaviour"};
    return type;
  }

  void onInit(Entity entity, Context& context) noexcept override
  {
    auto opt = context.getComponentManager().get<TransformComponent>(entity);
    if (opt.has_value())
      this->transform_ = std::addressof(*opt);
    else
      assert("No camera component found" == nullptr);
  }

  void onUpdate(Entity /*entity*/, UpdateContext& context) noexcept override
  {
    this->transform_->setAngle(
        this->transform_->getAngle()
        + static_cast<float>(context.getElapsedTime() * this->rotation_speed_));
  }

private:
  TransformComponent* transform_{nullptr};
  double rotation_speed_{1.0};
};

std::unique_ptr<Behaviour> make() noexcept
{
  return std::make_unique<RotationBehaviour>();
}
