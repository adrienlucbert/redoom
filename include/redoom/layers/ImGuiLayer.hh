#pragma once

#include <memory>
#include <unordered_map>

#include <redoom/Layer.hh>
#include <redoom/imgui/ImGuiWindow.hh>

namespace redoom
{
struct ImGuiLayer : public Layer {
  ImGuiLayer() noexcept;
  ~ImGuiLayer() noexcept override = default;

  void onAttach() noexcept override;

  void onDetach() noexcept override;

  void onUpdate(double elapsed_time) noexcept override;

  EventPropagation onEvent(events::Event const& event) noexcept override;

private:
  enum WindowID {
    Unknown,
    Scene,
    Properties,
    Log,
  };

  void showEditor() noexcept;
  bool onWindowFocus(WindowID window) noexcept;
  bool onWindowUnfocus(WindowID window) noexcept;

  std::unordered_map<WindowID, std::shared_ptr<ImGuiWindow>> windows_;

  WindowID focused_window_{Unknown};
};
} // namespace redoom
