#pragma once

#include <redoom/renderer/RendererAPI.hh>

class GLFWwindow;

namespace redoom::platform::OpenGL
{
class OpenGLRendererAPI : public renderer::RendererAPI
{
public:
  void setViewport(renderer::RendererViewport viewport) noexcept override;
  void setClearColor(const glm::vec4& color) noexcept override;
  void clear() noexcept override;

  [[nodiscard]] static Expected<std::unique_ptr<RendererAPI>> create() noexcept;

protected:
  OpenGLRendererAPI() noexcept = default;
};
} // namespace redoom::platform::OpenGL
