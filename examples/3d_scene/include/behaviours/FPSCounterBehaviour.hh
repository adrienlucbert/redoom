#include <iostream>

#include <redoom/ecs/Behaviour.hh>

struct FPSCounterBehaviour : public redoom::ecs::Behaviour {
  double elapsed_time{0.0};
  long elapsed_frames{0};

  void onUpdate(redoom::ecs::Entity /*entity*/,
      redoom::ecs::UpdateContext& context) noexcept override
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
