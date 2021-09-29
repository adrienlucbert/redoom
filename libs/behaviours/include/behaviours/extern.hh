#pragma once

#include <redoom/ecs/Behaviour.hh>

extern "C" {
std::unique_ptr<redoom::ecs::Behaviour> make() noexcept;
}
