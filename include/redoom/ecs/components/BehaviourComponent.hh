#pragma once

#include <memory>
#include <type_traits>

#include <Utils/Expected.hh>
#include <Utils/Factory.hh>
#include <redoom/ecs/Behaviour.hh>
#include <redoom/ecs/Component.hh>

namespace redoom::ecs::components
{
class BehaviourComponent : public Component<BehaviourComponent>
{
public:
  explicit BehaviourComponent(std::unique_ptr<Behaviour> behaviour) noexcept;
  BehaviourComponent(BehaviourComponent const&) noexcept = delete;
  BehaviourComponent(BehaviourComponent&&) noexcept = default;
  ~BehaviourComponent() noexcept override = default;

  BehaviourComponent& operator=(BehaviourComponent const&) noexcept = delete;
  BehaviourComponent& operator=(BehaviourComponent&&) noexcept = default;

  template <typename BehaviourType = Behaviour>
  [[nodiscard]] Expected<std::reference_wrapper<BehaviourType>> get()
      const noexcept
  {
    if constexpr (std::is_same_v<BehaviourType, Behaviour>) {
      return *this->behaviour_;
    } else {
      static_assert(std::is_base_of_v<Behaviour, BehaviourType>,
          "BehaviourType must inherit from BehaviourBase");
      auto ptr = std::dynamic_pointer_cast<BehaviourType>(this->behaviour_);
      if (ptr == nullptr)
        return tl::make_unexpected(
            "Invalid BehaviourBase to BehaviourType conversion");
      else
        return *ptr;
    }
  }

  void init(Entity entity, Context& context) const noexcept
  {
    this->get()->get().onInit(entity, context);
  }

  [[nodiscard]] std::string const& getType() const noexcept override;

  std::unique_ptr<Behaviour> behaviour_;
};
}; // namespace redoom::ecs::components
