#pragma once

#include <redoom/imgui/ImGuiWindow.hh>

namespace redoom
{
struct SceneImGuiWindow : public ImGuiWindow {
  ~SceneImGuiWindow() noexcept override = default;

  [[nodiscard]] std::string_view getName() const noexcept override;

  [[nodiscard]] std::vector<ImGuiStyleVarPair> const& getStyleVars()
      const noexcept override;

  void onUpdate() noexcept override;
};
} // namespace redoom
