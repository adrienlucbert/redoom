#pragma once

#include <redoom/imgui/FrameBufferImGuiWindow.hh>

namespace redoom
{
struct SceneImGuiWindow : public FrameBufferImGuiWindow {
  SceneImGuiWindow() noexcept;
  ~SceneImGuiWindow() noexcept override = default;

  [[nodiscard]] std::string_view getName() const noexcept override;

  void onFocusIn() noexcept override;
  void onFocusOut() noexcept override;
};
} // namespace redoom
