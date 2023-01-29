#include <redoom/imgui/SceneImGuiWindow.hh>

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
std::string_view SceneImGuiWindow::getName() const noexcept
{
  return "Scene";
}

SceneImGuiWindow::SceneImGuiWindow() noexcept
  : FrameBufferImGuiWindow{"default"}
{
}

void SceneImGuiWindow::onFocusIn() noexcept
{
  Runtime::get().getWindow().setCursorMode(
      renderer::Window::CursorMode::Hidden);
}

void SceneImGuiWindow::onFocusOut() noexcept
{
  Runtime::get().getWindow().setCursorMode(
      renderer::Window::CursorMode::Normal);
}
} // namespace redoom
