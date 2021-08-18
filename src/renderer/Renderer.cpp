#include <redoom/renderer/Renderer.hh>

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
auto Renderer::view_projection_matrix = glm::mat4{};

RendererAPI& Renderer::getAPI() noexcept
{
  return *Renderer::api;
}

void Renderer::setViewProjectionMatrix(glm::mat4 view) noexcept
{
  Renderer::view_projection_matrix = view;
}

glm::mat4 const& Renderer::getViewProjectionMatrix() noexcept
{
  return Renderer::view_projection_matrix;
}

void Renderer::draw(graphics::Program& program,
    graphics::Mesh& mesh,
    const glm::mat4& model) noexcept
{
  program.use();
  auto projection =
      glm::perspective(glm::radians(45.0f), 1600.0f / 900.0f, 0.1f, 100.0f);
  program.setUniformMatrix4(
      "projection", 1, GL_FALSE, glm::value_ptr(projection));
  program.setUniformMatrix4(
      "view", 1, GL_FALSE, glm::value_ptr(Renderer::getViewProjectionMatrix()));
  program.setUniformMatrix4("model", 1, GL_FALSE, glm::value_ptr(model));
  mesh.draw(program);
}
} // namespace redoom::renderer
