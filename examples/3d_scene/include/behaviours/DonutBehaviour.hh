#include <iostream>

#include <redoom/ecs/components/BehaviourComponent.hh>

struct DonutBehaviour : public redoom::ecs::components::Behaviour {
  void onUpdate(redoom::ecs::Entity entity,
      redoom::ecs::UpdateContext& context) noexcept override
  {
    std::cout << "NO PLEEEAAAAAASE! DON'T EAT MEEEEE!" << '\n';
  }
};
