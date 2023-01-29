#include <redoom/imgui/FrameBufferImGuiWindow.hh>

#include <iostream>

#include <imgui/imgui.h>
#include <imgui/imgui_impl_glfw.h>
#include <imgui/imgui_impl_opengl3.h>
#include <imgui/imgui_internal.h>

#include <redoom/Runtime.hh>
#include <redoom/graphics/FrameBufferLibrary.hh>
#include <redoom/renderer/Renderer.hh>
#include <redoom/renderer/Window.hh>

namespace redoom
{
std::string_view FrameBufferImGuiWindow::getName() const noexcept
{
  return "FrameBuffer";
}

FrameBufferImGuiWindow::FrameBufferImGuiWindow(
    std::string_view framebuffer_name) noexcept
  : framebuffer_name_{framebuffer_name}
{
}

void FrameBufferImGuiWindow::onUpdate() noexcept
{
  auto framebuffer_opt =
      graphics::FrameBufferLibrary::getFrameBuffer(this->framebuffer_name_);
  if (!framebuffer_opt.has_value()) {
    std::cerr << "Framebuffer " << this->framebuffer_name_ << " not available"
              << '\n';
    return;
  }
  auto& framebuffer = *framebuffer_opt;

  auto view = ImGui::GetContentRegionAvail();

  // clang-format off
	if (std::fabs(view.x - this->win_size_.x) > std::numeric_limits<float>::epsilon() ||
			std::fabs(view.y - this->win_size_.y) > std::numeric_limits<float>::epsilon()) {
    // clang-format on
    this->win_size_ = view;
    framebuffer.resize(static_cast<int>(view.x), static_cast<int>(view.y));
  }
  auto textureID = framebuffer.getTexture().getId();
  ImGui::Image(reinterpret_cast<void*>(textureID), // NOLINT
      this->win_size_,
      ImVec2{0, 1},
      ImVec2{1, 0});
}

std::vector<ImGuiStyleVarPair> const& FrameBufferImGuiWindow::getStyleVars()
    const noexcept
{
  static auto const& style = std::vector<ImGuiStyleVarPair>{
      {ImGuiStyleVar_WindowPadding, ImVec2(0.0f, 0.0f)}};
  return style;
}
} // namespace redoom
