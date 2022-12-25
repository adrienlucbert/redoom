#include <redoom/imgui/SceneImGuiWindow.hh>

#include <imgui/imgui.h>
#include <imgui/imgui_impl_glfw.h>
#include <imgui/imgui_impl_opengl3.h>
#include <imgui/imgui_internal.h>

#include <redoom/renderer/Renderer.hh>

namespace redoom
{
std::string_view SceneImGuiWindow::getName() const noexcept
{
  return "Scene";
}

void SceneImGuiWindow::onUpdate() noexcept
{
  static ImVec2 win_size = ImVec2{1000, 800};
  auto view = ImGui::GetContentRegionAvail();

  // clang-format off
	if (std::fabs(view.x - win_size.x) > std::numeric_limits<float>::epsilon() ||
			std::fabs(view.y - win_size.y) > std::numeric_limits<float>::epsilon()) {
    // clang-format on
    win_size = view;
    renderer::Renderer::get().getRenderPipeline().getFrameBuffer().resize(
        static_cast<int>(view.x), static_cast<int>(view.y));
  }
  uint64_t textureID = renderer::Renderer::get()
                           .getRenderPipeline()
                           .getFrameBuffer()
                           .getTexture()
                           .getId();
  ImGui::Image(reinterpret_cast<void*>(textureID), // NOLINT
      win_size,
      ImVec2{0, 1},
      ImVec2{1, 0});
}

std::vector<ImGuiStyleVarPair> const& SceneImGuiWindow::getStyleVars()
    const noexcept
{
  static auto const& style = std::vector<ImGuiStyleVarPair>{
      {ImGuiStyleVar_WindowPadding, ImVec2(0.0f, 0.0f)}};
  return style;
}
} // namespace redoom
