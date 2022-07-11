#include <iostream>

#include <redoom/graphics/Material.hh>
#include <redoom/graphics/Program.hh>
#include <redoom/graphics/UniformTypes.hh>

namespace redoom::graphics
{
Material::Material(std::string pshader,
    float pgloss,
    float psmoothness,
    float pmetallicness,
    glm::vec3 pcolor) noexcept
  : shader{std::move(pshader)}
  , gloss{pgloss}
  , smoothness{psmoothness}
  , metallicness{pmetallicness}
  , color{pcolor}
{
}

void Material::apply(Program& program) const noexcept
{
  program.setUniform("Gloss", uniforms::Float{.value = this->gloss});
  program.setUniform("Smoothness", uniforms::Float{.value = this->smoothness});
  program.setUniform(
      "Metallicness", uniforms::Float{.value = this->metallicness});
  program.setUniform("Color", uniforms::Vector<3>{.value = this->color});
}
} // namespace redoom::graphics
