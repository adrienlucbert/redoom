#pragma once

#include <redoom/ecs/Component.hh>

namespace mock
{
struct TestComponent1 final : public redoom::ecs::Component<TestComponent1> {
  [[nodiscard]] std::string const& getType() const noexcept override
  {
    static auto const type = std::string{"TestComponent1"};
    return type;
  }
};

struct TestComponent2 final : public redoom::ecs::Component<TestComponent2> {
  [[nodiscard]] std::string const& getType() const noexcept override
  {
    static auto const type = std::string{"TestComponent2"};
    return type;
  }
};
} // namespace mock
