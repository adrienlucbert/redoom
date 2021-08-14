#pragma once

#include <memory>
#include <type_traits>

#include <tl/expected.hpp>

#include <Utils/Expected.hh>
#include <redoom/ecs/Component.hh>
#include <redoom/ecs/UpdateContext.hh>

namespace redoom::ecs::components
{
class Behaviour
{
public:
  Behaviour() noexcept = default;
  Behaviour(Behaviour const& b) noexcept = delete;
  Behaviour(Behaviour&& b) noexcept = default;
  virtual ~Behaviour() noexcept = default;

  Behaviour& operator=(Behaviour const& rhs) noexcept = delete;
  Behaviour& operator=(Behaviour&& rhs) noexcept = default;

  virtual void onUpdate(Entity /*entity*/, UpdateContext& context) noexcept;
};

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

private:
  std::unique_ptr<Behaviour> behaviour;
};
}; // namespace redoom::ecs::components
