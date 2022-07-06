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

auto Renderer::global_uniforms_ = std::unordered_map<std::string, // NOLINT
    std::unique_ptr<graphics::UniformBase>>{};                    // NOLINT

RendererAPI& Renderer::getAPI() noexcept
{
  return *Renderer::api_;
}

void Renderer::prepareDraw(
    graphics::Program& program, const glm::mat4& model) noexcept
{
  program.use();
  program.setUniform("model",
      graphics::uniforms::Matrix<4, 4>{.value = model, .transpose = GL_FALSE});
  for (auto const& uniform : Renderer::global_uniforms_)
    uniform.second->set(program, uniform.first);
  program.apply();
}
} // namespace redoom::renderer
