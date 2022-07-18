#include <cstdlib>
#include <redoom/renderer/Renderer.hh>

#include <redoom/graphics/Camera.hh>

namespace redoom::renderer
{
Renderer::Renderer(std::unique_ptr<RendererAPI> api,
    std::unique_ptr<RenderPipeline> rp) noexcept
  : api_{std::move(api)}
  , rp_{std::move(rp)}
{
}

RendererAPI& Renderer::getAPI() noexcept
{
  return *this->api_;
}

void Renderer::useProgram(graphics::Program& program) noexcept
{
  this->current_program_ = program;
  program.use();
}

void Renderer::setTopology(GLenum topology) noexcept
{
  this->topology_ = topology;
}

GLenum Renderer::getTopology() const noexcept
{
  return this->topology_;
}

RenderPipeline const& Renderer::getRenderPipeline() const noexcept
{
  return *this->rp_;
}

void Renderer::beginRendering() noexcept
{
  this->rp_->beginRendering();
}

void Renderer::endRendering() noexcept
{
  this->rp_->endRendering();
}

// NOLINTNEXTLINE
void Renderer::useTextures(
    std::vector<std::reference_wrapper<graphics::Texture2D>> const& textures)
    const noexcept
{
  // Texture type: { Texture type label, Texture type count }
  auto texture_type_data = std::unordered_map<graphics::Texture2D::Type,
      std::pair<std::string, unsigned int>>{
      {graphics::Texture2D::Type::Diffuse, {"texture_diffuse", 0u}},
      {graphics::Texture2D::Type::Specular, {"texture_specular", 0u}},
      {graphics::Texture2D::Type::Ambient, {"texture_ambient", 0u}},
      {graphics::Texture2D::Type::Emissive, {"texture_emissive", 0u}},
      {graphics::Texture2D::Type::Height, {"texture_height", 0u}},
      {graphics::Texture2D::Type::Normals, {"texture_normals", 0u}},
      {graphics::Texture2D::Type::Shininess, {"texture_shininess", 0u}},
      {graphics::Texture2D::Type::Opacity, {"texture_opacity", 0u}},
      {graphics::Texture2D::Type::Displacement, {"texture_displacement", 0u}},
      {graphics::Texture2D::Type::Lightmap, {"texture_lightmap", 0u}},
      {graphics::Texture2D::Type::Reflection, {"texture_reflection", 0u}},
      {graphics::Texture2D::Type::BaseColor, {"texture_base_color", 0u}},
  };

  for (auto i = 0u; i < textures.size(); ++i) {
    auto const& texture = textures[i].get();
    auto const unit = static_cast<GLint>(i + 1);
    auto const& type_data = texture_type_data.find(texture.getType());
    if (type_data == texture_type_data.end()) {
      std::cerr << "Couldn't bind texture: unmapped texture type" << '\n';
      continue;
    }
    auto uniform = fmt::format(
        "{}{}", type_data->second.first, ++type_data->second.second);
    texture.setUnit(uniform, unit);
    texture.bind();
  }
}

void Renderer::prepareDraw(const glm::mat4& model) noexcept
{
  this->current_program_.value().use();
  this->current_program_.value().setUniform("model",
      graphics::uniforms::Matrix<4, 4>{.value = model, .transpose = GL_FALSE});
  for (auto const& uniform : this->global_uniforms_)
    uniform.second->set(this->current_program_.value(), uniform.first);
  this->current_program_.value().apply();
}
} // namespace redoom::renderer
