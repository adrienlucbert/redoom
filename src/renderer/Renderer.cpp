#include <redoom/renderer/Renderer.hh>

#include <redoom/graphics/Camera.hh>

namespace redoom::renderer
{
// NOLINTNEXTLINE
std::unique_ptr<RendererAPI> Renderer::api_ = []() {
  auto exp = RendererAPI::create();
  if (!exp)
    assert(exp.error().c_str());
  return std::move(*exp);
}();

// NOLINTNEXTLINE
auto Renderer::camera_view_matrix_ = glm::mat4{};       // NOLINT
auto Renderer::camera_projection_matrix_ = glm::mat4{}; // NOLINT

RendererAPI& Renderer::getAPI() noexcept
{
  return *Renderer::api_;
}

void Renderer::setViewMatrix(glm::mat4 view) noexcept
{
  Renderer::camera_view_matrix_ = view;
}

glm::mat4 const& Renderer::getViewMatrix() noexcept
{
  return Renderer::camera_view_matrix_;
}

void Renderer::setProjectionMatrix(glm::mat4 view) noexcept
{
  Renderer::camera_projection_matrix_ = view;
}

glm::mat4 const& Renderer::getProjectionMatrix() noexcept
{
  return Renderer::camera_projection_matrix_;
}
} // namespace redoom::renderer
