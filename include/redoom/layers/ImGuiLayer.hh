#pragma once

#include <redoom/Layer.hh>

namespace redoom
{
struct ImGuiLayer : public Layer {
  ~ImGuiLayer() noexcept override = default;

  void onAttach() noexcept override;

  void showEditor() noexcept;

  void onUpdate(double /*elapsed_time*/) noexcept override;

  void onDetach() noexcept override;
};
} // namespace redoom
