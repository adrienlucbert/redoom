#pragma once

#include <Utils/Expected.hh>
#include <Utils/Singleton.hh>
#include <redoom/Scene.hh>
#include <redoom/events/Event.hh>
#include <redoom/events/EventQueue.hh>
#include <redoom/renderer/Window.hh>

namespace redoom
{
class Runtime
{
public:
  Runtime(Runtime const&) noexcept = delete;
  Runtime(Runtime&&) noexcept = delete;
  ~Runtime() noexcept = default;

  Runtime& operator=(Runtime const&) noexcept = delete;
  Runtime& operator=(Runtime&&) noexcept = delete;

  static Runtime& get() noexcept;

  [[nodiscard]] bool shouldClose() const noexcept;

  [[nodiscard]] double getTime() const noexcept;

  [[nodiscard]] bool isReady() const noexcept;

  void onUpdate(double elapsed_time) noexcept;

  void afterUpdate() noexcept;

  void onEvent(events::Event const& event) noexcept;

  bool pollEvent(events::Event& buffer) noexcept;

  Scene& makeScene(std::string_view name, bool set_current = true) noexcept;
  Expected<std::reference_wrapper<Scene>> loadScene(
      std::string_view filepath, bool set_current = true) noexcept;
  [[nodiscard]] Scene& getCurrentScene() noexcept;
  void setCurrentScene(std::string const& name) noexcept;

  [[nodiscard]] renderer::Window& getWindow() noexcept;

protected:
  explicit Runtime(std::string_view title,
      renderer::EventCallback const& event_callback) noexcept;

  friend class Application;

private:
  Expected<> init(std::string_view title,
      renderer::EventCallback const& event_callback) noexcept;

  std::unique_ptr<renderer::Window> window_;
  std::shared_ptr<Scene> current_scene_;
  std::unordered_map<std::string, std::shared_ptr<Scene>> scenes_;
  events::EventQueue events_;
};
} // namespace redoom
