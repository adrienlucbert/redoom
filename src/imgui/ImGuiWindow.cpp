#include <redoom/imgui/ImGuiWindow.hh>

#include <imgui/imgui.h>
#include <imgui/imgui_impl_glfw.h>
#include <imgui/imgui_impl_opengl3.h>
#include <imgui/imgui_internal.h>
#include <variant>

namespace redoom
{
void ImGuiStyleVarPair::apply() const noexcept
{
  std::visit([&](auto&& v) { ImGui::PushStyleVar(this->idx, v); }, this->value);
}

std::vector<ImGuiStyleVarPair> const& ImGuiWindow::getStyleVars() const noexcept
{
  static auto const& style = std::vector<ImGuiStyleVarPair>{};
  return style;
}
} // namespace redoom
