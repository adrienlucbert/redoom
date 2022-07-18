#include <redoom/graphics/Material.hh>

#include <redoom/graphics/ShaderLibrary.hh>
#include <redoom/graphics/UniformTypes.hh>
#include <redoom/renderer/Renderer.hh>

namespace redoom::graphics
{
Material::Material(std::string shader,
    float gloss,
    float smoothness,
    float metallicness,
    glm::vec3 color,
    std::vector<std::reference_wrapper<Texture2D>> textures,
    unsigned int topology) noexcept
  : shader_{std::move(shader)}
  , gloss_{gloss}
  , smoothness_{smoothness}
  , metallicness_{metallicness}
  , color_{color}
  , textures_{std::move(textures)}
  , topology_{topology}
{
}

void Material::use() const noexcept
{
  auto program_opt = ShaderLibrary::getShader(this->shader_);
  if (!program_opt)
    assert("Undefined program" == nullptr);
  auto& program = *program_opt;
  renderer::Renderer::get().useProgram(program);
  program.setUniform("Gloss", uniforms::Float{.value = this->gloss_});
  program.setUniform("Smoothness", uniforms::Float{.value = this->smoothness_});
  program.setUniform(
      "Metallicness", uniforms::Float{.value = this->metallicness_});
  program.setUniform("Color", uniforms::Vector<3>{.value = this->color_});
  renderer::Renderer::get().setTopology(this->topology_);
  if (!this->textures_.empty()) {
    renderer::Renderer::get().useTextures(this->textures_);
  } else {
    renderer::Renderer::get().useTextures({*Texture2D::getPlaceholder()});
  }
}
} // namespace redoom::graphics
