#pragma once

#include <redoom/ecs/Component.hh>

namespace mock
{
struct TestComponent1 final : public redoom::ecs::Component<TestComponent1> {
};

struct TestComponent2 final : public redoom::ecs::Component<TestComponent2> {
};
} // namespace mock
