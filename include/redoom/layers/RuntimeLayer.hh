#pragma once

#include <redoom/Layer.hh>

namespace redoom
{
struct RuntimeLayer : public Layer {
  ~RuntimeLayer() noexcept override = default;

  void onUpdate(double elapsed_time) noexcept override;

  void afterUpdate() noexcept override;

  EventPropagation onEvent(events::Event const& event) noexcept override;
};
} // namespace redoom
