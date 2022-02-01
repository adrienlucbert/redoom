#pragma once

#include <redoom/renderer/RendererAPI.hh>

class GLFWwindow;

namespace redoom::platform::OpenGL
{
class OpenGLRendererAPI : public renderer::RendererAPI
{
public:
  [[nodiscard]] bool isWireframe() const noexcept override;
  void setWireframe(bool is_wireframe) noexcept override;
  void setViewport(renderer::RendererViewport viewport) noexcept override;
  void setClearColor(const glm::vec4& color) noexcept override;
  void clear() noexcept override;

  [[nodiscard]] static Expected<std::unique_ptr<RendererAPI>> create() noexcept;

protected:
  OpenGLRendererAPI() noexcept = default;

private:
  bool is_wireframe{false};
};
} // namespace redoom::platform::OpenGL
