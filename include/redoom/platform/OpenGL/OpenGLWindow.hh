#pragma once

#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include <redoom/events/EventQueue.hh>
#include <redoom/renderer/RendererContext.hh>
#include <redoom/renderer/Window.hh>

namespace redoom::platform::OpenGL
{
class OpenGLWindow : public renderer::Window
{
public:
  OpenGLWindow(OpenGLWindow const& b) noexcept = delete;
  OpenGLWindow(OpenGLWindow&& b) noexcept = default;
  ~OpenGLWindow() noexcept override;

  OpenGLWindow& operator=(OpenGLWindow const& rhs) noexcept = delete;
  OpenGLWindow& operator=(OpenGLWindow&& rhs) noexcept = default;

  [[nodiscard]] int getWidth() const noexcept override;
  [[nodiscard]] int getHeight() const noexcept override;

  void setVSync(bool phas_vsync) noexcept override;
  [[nodiscard]] bool hasVSync() noexcept override;

  [[nodiscard]] void* getNativeWindow() const noexcept override;

  void setCursorMode(CursorMode mode) noexcept override;

  [[nodiscard]] bool pollEvent(events::Event& buffer) noexcept override;

  void onUpdate() noexcept override;

  [[nodiscard]] static Expected<std::unique_ptr<renderer::Window>> create(
      std::string_view title, int width, int height) noexcept;

protected:
  OpenGLWindow(GLFWwindow* pwindow,
      std::unique_ptr<renderer::RendererContext> pcontext,
      int pwidth,
      int pheight) noexcept;

  int width;
  int height;
  GLFWwindow* window;
  std::unique_ptr<renderer::RendererContext> context;
  bool has_vsync;
  events::EventQueue events;
};
} // namespace redoom::platform::OpenGL
