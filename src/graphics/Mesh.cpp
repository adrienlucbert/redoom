#include <redoom/graphics/Mesh.hh>

#include <iostream>

namespace redoom::graphics
{
Mesh::Mesh(std::vector<Vertex> pvertices,
    std::vector<unsigned int> pindices,
    std::vector<Texture2D> ptextures,
    GLenum ptopology) noexcept
  : vertices{std::move(pvertices)}
  , indices{std::move(pindices)}
  , textures{std::move(ptextures)}
  , topology{ptopology}
  , vbo{this->vertices, BufferUsage::STATIC}
  , ebo{this->indices, BufferUsage::STATIC}
{
  if (this->textures.empty()) {
    auto exp = Texture2D::getPlaceholder();
    if (!exp)
      std::cerr << "Warning: Could not generate texture placeholder" << '\n';
    else
      this->textures.push_back(std::move(*exp));
  }

  this->vao.bind();
  this->vbo.bind();
  this->ebo.bind();
  this->vbo.setLayout({{ShaderDataType::Float3, "aPos"},
      {ShaderDataType::Float3, "aNormal"},
      {ShaderDataType::Float3, "aColor"},
      {ShaderDataType::Float2, "aTexCoord"}});
  this->vao.unbind();
  this->vbo.unbind();
  this->ebo.unbind();
}

void Mesh::draw(Program& program) const noexcept
{
  program.use();
  this->vao.bind();

  // Texture type: { Texture type label, Texture type count }
  auto texture_type_data =
      std::unordered_map<Texture2D::Type, std::pair<std::string, unsigned int>>{
          {Texture2D::Type::Diffuse, {"texture_diffuse", 1u}},
          {Texture2D::Type::Specular, {"texture_specular", 1u}},
          {Texture2D::Type::Ambient, {"texture_ambient", 1u}},
          {Texture2D::Type::Emissive, {"texture_emissive", 1u}},
          {Texture2D::Type::Height, {"texture_height", 1u}},
          {Texture2D::Type::Normals, {"texture_normals", 1u}},
          {Texture2D::Type::Shininess, {"texture_shininess", 1u}},
          {Texture2D::Type::Opacity, {"texture_opacity", 1u}},
          {Texture2D::Type::Displacement, {"texture_displacement", 1u}},
          {Texture2D::Type::Lightmap, {"texture_lightmap", 1u}},
          {Texture2D::Type::Reflection, {"texture_reflection", 1u}},
          {Texture2D::Type::BaseColor, {"texture_base_color", 1u}},
      };

  for (auto i = 0u; i < this->textures.size(); ++i) {
    auto const& texture = this->textures[i];
    auto const unit = static_cast<GLint>(i + 1);
    auto const& type_data = texture_type_data.find(texture.getType());
    if (type_data == texture_type_data.end()) {
      std::cerr << "Couldn't bind texture: unmapped texture type" << '\n';
      continue;
    }
    auto uniform = fmt::format(
        "{}{}", type_data->second.first, type_data->second.second++);
    texture.setUnit(program, uniform, unit);
    texture.bind();
  }

  if (!this->indices.empty())
    this->ebo.draw(this->topology);
  else
    this->vbo.draw(this->topology);

  for (auto const& texture : this->textures)
    texture.unbind();
  this->vao.unbind();
}

std::vector<Vertex> const& Mesh::getVertices() const noexcept
{
  return this->vertices;
}
} // namespace redoom::graphics
