#pragma once

#include <string_view>

#include <Utils/Expected.hh>
#include <Utils/Singleton.hh>
#include <redoom/Scene.hh>
#include <redoom/ecs/Registry.hh>
#include <redoom/renderer/Renderer.hh>
#include <redoom/renderer/RendererContext.hh>
#include <redoom/renderer/Window.hh>

class GLFWwindow;

namespace redoom
{
struct ApplicationArguments {
  int argc;
  char** argv;

  char const* operator[](int index) const
  {
    assert(index < this->argc);
    return this->argv[index];
  }
};

class Application : public Utils::Singleton<Application>
{
public:
  Application(Application const& b) noexcept = delete;
  Application(Application&& b) noexcept = delete;
  ~Application() noexcept override = default;

  Application& operator=(Application const& rhs) noexcept = delete;
  Application& operator=(Application&& rhs) noexcept = delete;

  [[nodiscard]] bool isReady() const noexcept;
  void run() noexcept;

  Scene& makeScene(std::string_view name, bool set_current = true) noexcept;
  Expected<std::reference_wrapper<Scene>> loadScene(
      std::string_view filepath, bool set_current = true) noexcept;
  [[nodiscard]] Scene& getCurrentScene() noexcept;
  void setCurrentScene(std::string const& name) noexcept;

  [[nodiscard]] renderer::Window& getWindow() noexcept;
  [[nodiscard]] ApplicationArguments const& getArgs() const noexcept;

protected:
  Application(std::string_view title, ApplicationArguments args) noexcept;

  ApplicationArguments args_;
  double previous_time_{0.0};
  std::unique_ptr<renderer::Window> window_;
  std::shared_ptr<Scene> current_scene_;
  std::unordered_map<std::string, std::shared_ptr<Scene>> scenes_;
};

std::unique_ptr<Application> createApplication(ApplicationArguments args);
}; // namespace redoom
