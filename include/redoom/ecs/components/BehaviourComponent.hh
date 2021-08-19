#pragma once

#include <memory>
#include <type_traits>

#include <Utils/Expected.hh>
#include <redoom/ecs/Behaviour.hh>
#include <redoom/ecs/Component.hh>

namespace redoom::ecs::components
{
class BehaviourComponent : public Component<BehaviourComponent>
{
public:
  explicit BehaviourComponent(std::unique_ptr<Behaviour> pbehaviour) noexcept;
  BehaviourComponent(BehaviourComponent const& b) noexcept = delete;
  BehaviourComponent(BehaviourComponent&& b) noexcept = default;
  ~BehaviourComponent() noexcept override = default;

  BehaviourComponent& operator=(
      BehaviourComponent const& rhs) noexcept = delete;
  BehaviourComponent& operator=(BehaviourComponent&& rhs) noexcept = default;

  template <typename BehaviourType = Behaviour>
  [[nodiscard]] Expected<std::reference_wrapper<BehaviourType>> get()
      const noexcept
  {
    if constexpr (std::is_same_v<BehaviourType, Behaviour>) {
      return *this->behaviour;
    } else {
      static_assert(std::is_base_of_v<Behaviour, BehaviourType>,
          "BehaviourType must inherit from BehaviourBase");
      auto ptr = std::dynamic_pointer_cast<BehaviourType>(this->behaviour);
      if (ptr == nullptr)
        return tl::make_unexpected(
            "Invalid BehaviourBase to BehaviourType conversion");
      else
        return *ptr;
    }
  }

  void onInit(Entity entity, Context& context) noexcept override;
  void onDestroy(Entity entit, Context& contexty) noexcept override;

private:
  std::unique_ptr<Behaviour> behaviour;
};
}; // namespace redoom::ecs::components
