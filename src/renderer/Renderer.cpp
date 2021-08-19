#include <redoom/renderer/Renderer.hh>

#include <redoom/graphics/Camera.hh>

namespace redoom::renderer
{
// NOLINTNEXTLINE
std::unique_ptr<RendererAPI> Renderer::api = []() {
  auto exp = RendererAPI::create();
  if (!exp)
    assert(exp.error().c_str());
  return std::move(*exp);
}();

// NOLINTNEXTLINE
auto Renderer::camera_view_matrix = glm::mat4{};       // NOLINT
auto Renderer::camera_projection_matrix = glm::mat4{}; // NOLINT

RendererAPI& Renderer::getAPI() noexcept
{
  return *Renderer::api;
}

void Renderer::setViewMatrix(glm::mat4 view) noexcept
{
  Renderer::camera_view_matrix = view;
}

glm::mat4 const& Renderer::getViewMatrix() noexcept
{
  return Renderer::camera_view_matrix;
}

void Renderer::setProjectionMatrix(glm::mat4 view) noexcept
{
  Renderer::camera_projection_matrix = view;
}

glm::mat4 const& Renderer::getProjectionMatrix() noexcept
{
  return Renderer::camera_projection_matrix;
}

void Renderer::draw(graphics::Program& program,
    graphics::Mesh& mesh,
    const glm::mat4& model) noexcept
{
  program.use();
  program.setUniformMatrix4("projection",
      1,
      GL_FALSE,
      glm::value_ptr(Renderer::getProjectionMatrix()));
  program.setUniformMatrix4(
      "view", 1, GL_FALSE, glm::value_ptr(Renderer::getViewMatrix()));
  program.setUniformMatrix4("model", 1, GL_FALSE, glm::value_ptr(model));
  mesh.draw(program);
}
} // namespace redoom::renderer
