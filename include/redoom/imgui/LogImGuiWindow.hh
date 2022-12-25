#pragma once

#include <redoom/imgui/ImGuiWindow.hh>

namespace redoom
{
struct LogImGuiWindow : public ImGuiWindow {
  ~LogImGuiWindow() noexcept override = default;

  [[nodiscard]] std::string_view getName() const noexcept override;

  void onUpdate() noexcept override;
};
} // namespace redoom
