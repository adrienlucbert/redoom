#pragma once

#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include <tl/optional.hpp>

#include <redoom/events/EventQueue.hh>
#include <redoom/graphics/FrameBuffer.hh>
#include <redoom/renderer/RendererContext.hh>
#include <redoom/renderer/Window.hh>

namespace redoom::platform::OpenGL
{
class OpenGLWindow : public renderer::Window
{
public:
  OpenGLWindow(OpenGLWindow const&) noexcept = delete;
  OpenGLWindow(OpenGLWindow&&) noexcept = default;
  ~OpenGLWindow() noexcept override;

  OpenGLWindow& operator=(OpenGLWindow const&) noexcept = delete;
  OpenGLWindow& operator=(OpenGLWindow&&) noexcept = default;

  [[nodiscard]] int getWidth() const noexcept override;
  [[nodiscard]] int getHeight() const noexcept override;

  void setVSync(bool has_vsync) noexcept override;
  [[nodiscard]] bool hasVSync() noexcept override;

  [[nodiscard]] void* getNativeWindow() const noexcept override;

  void setCursorMode(CursorMode mode) noexcept override;

  [[nodiscard]] bool pollEvent(events::Event& buffer) noexcept override;

  void onUpdate() noexcept override;

  [[nodiscard]] static Expected<std::unique_ptr<renderer::Window>> create(
      std::string_view title, int width, int height) noexcept;

protected:
  OpenGLWindow(GLFWwindow* window,
      std::unique_ptr<renderer::RendererContext> context,
      int width,
      int height) noexcept;

  int width_;
  int height_;
  GLFWwindow* window_;
  std::unique_ptr<renderer::RendererContext> context_;
  bool has_vsync_;
  events::EventQueue events_;
};
} // namespace redoom::platform::OpenGL
