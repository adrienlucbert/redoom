#include <redoom/layers/ImGuiLayer.hh>

#include <imgui/imgui.h>
#include <imgui/imgui_impl_glfw.h>
#include <imgui/imgui_impl_opengl3.h>
#include <imgui/imgui_internal.h>

#include <redoom/Runtime.hh>
#include <redoom/events/Key.hh>
#include <redoom/imgui/FrameBufferImGuiWindow.hh>
#include <redoom/imgui/ImGuiWindow.hh>
#include <redoom/imgui/LogImGuiWindow.hh>
#include <redoom/imgui/PropertiesImGuiWindow.hh>
#include <redoom/imgui/SceneImGuiWindow.hh>

namespace redoom
{
ImGuiLayer::ImGuiLayer() noexcept
  : windows_{
      {WindowID::Scene, std::make_shared<SceneImGuiWindow>()},
      {WindowID::FrameBuffer,
          std::make_shared<FrameBufferImGuiWindow>("mouse_picking")},
      {WindowID::Properties, std::make_shared<PropertiesImGuiWindow>()},
      {WindowID::Log, std::make_shared<LogImGuiWindow>()},
  }
{
}

void ImGuiLayer::onAttach() noexcept
{
  IMGUI_CHECKVERSION();
  ImGui::CreateContext();

  // ImGui Config
  auto& io = ImGui::GetIO();
  io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;

  // ImGui Style
  ImGui::StyleColorsDark();

  // ImGui Init
  auto* native_window =
      static_cast<GLFWwindow*>(Runtime::get().getWindow().getNativeWindow());

  ImGui_ImplGlfw_InitForOpenGL(native_window, true);
  ImGui_ImplOpenGL3_Init("#version 330");
}

void ImGuiLayer::onDetach() noexcept
{
  ImGui_ImplOpenGL3_Shutdown();
  ImGui_ImplGlfw_Shutdown();
  ImGui::DestroyContext();
}

void ImGuiLayer::onUpdate(double /*elapsed_time*/) noexcept
{
  ImGui_ImplOpenGL3_NewFrame();
  ImGui_ImplGlfw_NewFrame();
  ImGui::NewFrame();
  this->showEditor();
  auto& io = ImGui::GetIO();
  auto& window = Runtime::get().getWindow();
  io.DisplaySize = ImVec2(static_cast<float>(window.getWidth()),
      static_cast<float>(window.getHeight()));
  ImGui::Render();
  ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
}

EventPropagation ImGuiLayer::onEvent(events::Event const& /*event*/) noexcept
{
  if (this->focused_window_ == WindowID::Scene) {
    if (events::isKeyPressed(events::Key::ESCAPE)) {
      ImGui::FocusWindow(nullptr);
      this->onWindowUnfocus(WindowID::Scene);
    }
    return Forward;
  }
  return Halt;
}

void ImGuiLayer::showEditor() noexcept
{
  static ImGuiDockNodeFlags dockspace_flags = ImGuiDockNodeFlags_None;

  ImGuiWindowFlags window_flags =
      ImGuiWindowFlags_MenuBar | ImGuiWindowFlags_NoDocking;
  const ImGuiViewport* viewport = ImGui::GetMainViewport();
  ImGui::SetNextWindowPos(viewport->WorkPos);
  ImGui::SetNextWindowSize(viewport->WorkSize);
  ImGui::SetNextWindowViewport(viewport->ID);
  window_flags |= ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoCollapse
                | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove
                | ImGuiWindowFlags_NoBringToFrontOnFocus
                | ImGuiWindowFlags_NoNavFocus;
  auto dockspace_id = ImGui::GetID("Main Dockspace");

  if ((dockspace_flags & ImGuiDockNodeFlags_PassthruCentralNode) != 0)
    window_flags |= ImGuiWindowFlags_NoBackground;

  // Main dockspace
  ImGui::PushStyleVar(ImGuiStyleVar_WindowRounding, 0.0f);
  ImGui::PushStyleVar(ImGuiStyleVar_WindowBorderSize, 0.0f);
  ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0.0f, 0.0f));
  ImGui::Begin("Redoom Editor", nullptr, window_flags);
  {
    ImGui::PopStyleVar(3);

    ImGuiIO& io = ImGui::GetIO();
    if ((io.ConfigFlags & ImGuiConfigFlags_DockingEnable) != 0) {
      ImGui::DockSpace(dockspace_id, ImVec2(0.0f, 0.0f), dockspace_flags);
    }

    if (ImGui::BeginMenuBar()) {
      if (ImGui::BeginMenu("File")) {
        if (ImGui::MenuItem("Save")) {
          std::cout << "Save" << '\n';
        }
        ImGui::EndMenu();
      }
      ImGui::EndMenuBar();
    }

    if (ImGui::BeginMenuBar()) {
      if (ImGui::BeginMenu("Windows")) {
        for (auto const& window : this->windows_) {
          if (ImGui::MenuItem(window.second->getName().data())) {
            std::cout << "Toggle " << window.second->getName() << '\n';
          }
        }
        ImGui::EndMenu();
      }
      ImGui::EndMenuBar();
    }
  }
  ImGui::End();

  for (auto const& window : this->windows_) {
    auto const& styleVars = window.second->getStyleVars();
    for (auto const& styleVar : styleVars)
      styleVar.apply();
    ImGui::Begin(window.second->getName().data());
    ImGui::PopStyleVar(static_cast<int>(styleVars.size()));
    if (ImGui::IsWindowFocused())
      this->onWindowFocus(window.first);
    window.second->onUpdate();
    ImGui::End();
  }
}

bool ImGuiLayer::onWindowFocus(WindowID window) noexcept
{
  auto const focusInWindowIt = this->windows_.find(window);
  if (focusInWindowIt == this->windows_.end())
    return false;
  auto focusOutWindowIt = this->windows_.find(window);
  if (focusOutWindowIt != this->windows_.end())
    focusOutWindowIt->second->onFocusOut();
  focusInWindowIt->second->onFocusIn();
  this->focused_window_ = window;
  return true;
}

bool ImGuiLayer::onWindowUnfocus(WindowID window) noexcept
{
  auto focusOutWindowIt = this->windows_.find(window);
  if (focusOutWindowIt == this->windows_.end())
    return false;
  focusOutWindowIt->second->onFocusOut();
  this->focused_window_ = WindowID::Unknown;
  return true;
}
} // namespace redoom
