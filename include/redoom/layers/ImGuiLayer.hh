#pragma once

#include <redoom/Layer.hh>

namespace redoom
{
struct ImGuiLayer : public Layer {
  ~ImGuiLayer() noexcept override = default;

  void onAttach() noexcept override;

  void onDetach() noexcept override;

  void onUpdate(double elapsed_time) noexcept override;

  void onEvent(events::Event const& event) noexcept override;

protected:
  void showEditor() noexcept;
};
} // namespace redoom
