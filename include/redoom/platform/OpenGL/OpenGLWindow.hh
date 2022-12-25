#pragma once

#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include <tl/optional.hpp>

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

  void setEventCallback(
      renderer::EventCallback const& callback) noexcept override;

  void onUpdate() noexcept override;

  [[nodiscard]] static Expected<std::unique_ptr<renderer::Window>> create(
      std::string_view title,
      int width,
      int height,
      renderer::EventCallback const& event_callback = nullptr) noexcept;

protected:
  OpenGLWindow(GLFWwindow* window,
      std::unique_ptr<renderer::RendererContext> context,
      int width,
      int height,
      renderer::EventCallback event_callback = nullptr) noexcept;

  void dispatchEvent(events::Event const& e) const noexcept;

  int width_;
  int height_;
  GLFWwindow* window_;
  std::unique_ptr<renderer::RendererContext> context_;
  bool has_vsync_;
  renderer::EventCallback event_callback_;
};
} // namespace redoom::platform::OpenGL
