#include <behaviours/extern.hh>

#include <iostream>

#include <redoom/ecs/Behaviour.hh>

using redoom::ecs::Behaviour;
using redoom::ecs::Entity;
using redoom::ecs::UpdateContext;

struct FPSCounterBehaviour : public Behaviour {
  double elapsed_time{0.0};
  long elapsed_frames{0};

  [[nodiscard]] std::string const& getType() const noexcept override
  {
    static auto const type = std::string{"FPSCounterBehaviour"};
    return type;
  }

  void onUpdate(Entity /*entity*/, UpdateContext& context) noexcept override
  {
    this->elapsed_time += context.elapsed_time;
    ++this->elapsed_frames;
    if (this->elapsed_time >= 1.0) {
      auto fps = static_cast<long>(
          static_cast<double>(this->elapsed_frames) / this->elapsed_time);
      std::cout << fps << '\n';
      this->elapsed_frames = 0;
      this->elapsed_time = 0.0;
    }
  }
};

std::unique_ptr<Behaviour> make() noexcept
{
  return std::make_unique<FPSCounterBehaviour>();
}
