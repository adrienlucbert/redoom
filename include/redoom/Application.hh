#pragma once

#include <string_view>

#include <Utils/Expected.hh>
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

class Application
{
public:
  Application(Application const& b) noexcept = delete;
  Application(Application&& b) noexcept = default;
  ~Application() noexcept = default;

  Application& operator=(Application const& rhs) noexcept = delete;
  Application& operator=(Application&& rhs) noexcept = default;

  [[nodiscard]] bool isReady() const noexcept;
  void run() noexcept;

  [[nodiscard]] ecs::Registry& getRegistry() noexcept;

  [[nodiscard]] renderer::Window& getWindow() noexcept;

  ApplicationArguments args;

protected:
  explicit Application(
      std::string_view title, ApplicationArguments args) noexcept;

  double previous_time{0.0};
  std::unique_ptr<renderer::Window> window;
  ecs::Registry registry{};
};

std::unique_ptr<Application> createApplication(ApplicationArguments args);
}; // namespace redoom
