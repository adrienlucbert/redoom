#include <redoom/layers/ImGuiLayer.hh>

#include <array>

#include <imgui/imgui.h>
#include <imgui/imgui_impl_glfw.h>
#include <imgui/imgui_impl_opengl3.h>
#include <imgui/imgui_internal.h>

#include <redoom/Runtime.hh>
#include <redoom/imgui/ImGuiWindow.hh>
#include <redoom/imgui/LogImGuiWindow.hh>
#include <redoom/imgui/PropertiesImGuiWindow.hh>
#include <redoom/imgui/SceneImGuiWindow.hh>

namespace redoom
{
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

void ImGuiLayer::onEvent(events::Event const& /*event*/) noexcept
{
}

void ImGuiLayer::showEditor() noexcept
{
  static ImGuiDockNodeFlags dockspace_flags = ImGuiDockNodeFlags_None;
  static auto const windows = std::array<std::unique_ptr<ImGuiWindow>, 3>{
      std::make_unique<SceneImGuiWindow>(),
      std::make_unique<PropertiesImGuiWindow>(),
      std::make_unique<LogImGuiWindow>(),
  };

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
        for (auto const& window : windows) {
          if (ImGui::MenuItem(window->getName().data())) {
            std::cout << "Toggle " << window->getName() << '\n';
          }
        }
        ImGui::EndMenu();
      }
      ImGui::EndMenuBar();
    }
  }
  ImGui::End();

  for (auto const& window : windows) {
    auto const& styleVars = window->getStyleVars();
    for (auto const& styleVar : styleVars)
      styleVar.apply();
    ImGui::Begin(window->getName().data());
    ImGui::PopStyleVar(static_cast<int>(styleVars.size()));
    window->onUpdate();
    ImGui::End();
  }
}
} // namespace redoom
