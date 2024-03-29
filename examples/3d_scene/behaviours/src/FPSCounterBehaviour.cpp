#include <behaviours/extern.hh>

#include <iostream>

#include <redoom/ecs/Behaviour.hh>

using redoom::ecs::Behaviour;
using redoom::ecs::Entity;
using redoom::ecs::UpdateContext;
using redoom::ecs::components::BehaviourComponent;

struct FPSCounterBehaviour : public Behaviour {
  double elapsed_time_{0.0};
  long elapsed_frames_{0};

  [[nodiscard]] std::string const& getType() const noexcept override
  {
    static auto const type = std::string{"FPSCounterBehaviour"};
    return type;
  }

  void onUpdate(Entity /*entity*/, UpdateContext& context) noexcept override
  {
    this->elapsed_time_ += context.getElapsedTime();
    ++this->elapsed_frames_;
    if (this->elapsed_time_ >= 1.0) {
      auto fps = static_cast<long>(
          static_cast<double>(this->elapsed_frames_) / this->elapsed_time_);
      std::cout << fps << '\n';
      this->elapsed_frames_ = 0;
      this->elapsed_time_ = 0.0;
    }
  }
};

void serialize(YAML::Emitter& /*out*/, BehaviourComponent const* /*component*/)
{
}

redoom::Expected<std::unique_ptr<Behaviour>> deserialize(
    YAML::Node const& /*node*/,
    redoom::Scene& /*scene*/,
    redoom::ecs::Entity /*entity*/)
{
  return std::make_unique<FPSCounterBehaviour>();
}
