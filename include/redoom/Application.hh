#pragma once

#include <string_view>

#include <Utils/Expected.hh>
#include <Utils/Singleton.hh>
#include <redoom/Layer.hh>
#include <redoom/Runtime.hh>
#include <redoom/Scene.hh>
#include <redoom/ecs/Registry.hh>
#include <redoom/events/EventQueue.hh>
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
  Application(Application const&) noexcept = delete;
  Application(Application&&) noexcept = delete;
  ~Application() noexcept override;

  Application& operator=(Application const&) noexcept = delete;
  Application& operator=(Application&&) noexcept = delete;

  [[nodiscard]] bool isReady() const noexcept;
  void run() noexcept;

  void onEvent(events::Event const& event) noexcept;

  void pushLayer(std::shared_ptr<Layer> layer) noexcept;
  void popLayer(std::shared_ptr<Layer> const& layer) noexcept;

  [[nodiscard]] Runtime& getRuntime() noexcept;

  [[nodiscard]] ApplicationArguments const& getArgs() const noexcept;

protected:
  Application(std::string_view title, ApplicationArguments args) noexcept;

  ApplicationArguments args_;
  double previous_time_{0.0};
  std::vector<std::shared_ptr<Layer>> layers_;
  Runtime runtime_;
};

std::unique_ptr<Application> createApplication(ApplicationArguments args);
}; // namespace redoom
