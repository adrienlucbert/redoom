#pragma once

#include <string_view>
#include <variant>
#include <vector>

#include <imgui/imgui.h>

namespace redoom
{
struct ImGuiStyleVarPair {
  ImGuiStyleVar idx;
  std::variant<ImVec2, float> value;

  void apply() const noexcept;
};

class ImGuiWindow
{
public:
  ImGuiWindow() noexcept = default;
  ImGuiWindow(ImGuiWindow const&) noexcept = delete;
  ImGuiWindow(ImGuiWindow&&) noexcept = default;
  virtual ~ImGuiWindow() noexcept = default;

  ImGuiWindow& operator=(ImGuiWindow const&) noexcept = delete;
  ImGuiWindow& operator=(ImGuiWindow&&) noexcept = default;

  [[nodiscard]] virtual std::string_view getName() const noexcept = 0;
  [[nodiscard]] virtual std::vector<ImGuiStyleVarPair> const& getStyleVars()
      const noexcept;

  virtual void onUpdate() noexcept = 0;
};
} // namespace redoom
