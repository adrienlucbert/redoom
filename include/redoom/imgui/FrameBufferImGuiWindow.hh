#pragma once

#include <string>
#include <string_view>

#include <redoom/graphics/FrameBuffer.hh>
#include <redoom/imgui/ImGuiWindow.hh>

namespace redoom
{
struct FrameBufferImGuiWindow : public ImGuiWindow {
  explicit FrameBufferImGuiWindow(std::string_view framebuffer_name) noexcept;
  ~FrameBufferImGuiWindow() noexcept override = default;

  [[nodiscard]] std::string_view getName() const noexcept override;

  [[nodiscard]] std::vector<ImGuiStyleVarPair> const& getStyleVars()
      const noexcept override;

  void onUpdate() noexcept override;

private:
  ImVec2 win_size_{1000, 800};
  std::string framebuffer_name_;
};
} // namespace redoom
