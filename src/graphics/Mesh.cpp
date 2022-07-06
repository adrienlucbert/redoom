#include <redoom/graphics/Mesh.hh>

#include <iostream>

namespace redoom::graphics
{
Mesh::Mesh(std::vector<Vertex> vertices,
    std::vector<unsigned int> indices,
    std::vector<Texture2D> textures,
    GLenum topology) noexcept
  : vertices_{std::move(vertices)}
  , indices_{std::move(indices)}
  , textures_{std::move(textures)}
  , topology_{topology}
  , vbo_{this->vertices_, BufferUsage::STATIC}
  , ebo_{this->indices_, BufferUsage::STATIC}
{
  if (this->textures_.empty()) {
    auto exp = Texture2D::getPlaceholder();
    if (!exp)
      std::cerr << "Warning: Could not generate texture placeholder" << '\n';
    else
      this->textures_.push_back(std::move(*exp));
  }

  this->vao_.bind();
  this->vbo_.bind();
  this->ebo_.bind();
  this->vbo_.setLayout({{ShaderDataType::Float3, "aPos"},
      {ShaderDataType::Float3, "aNormal"},
      {ShaderDataType::Float3, "aColor"},
      {ShaderDataType::Float2, "aTexCoord"}});
  this->vao_.unbind();
  this->vbo_.unbind();
  this->ebo_.unbind();
}

void Mesh::draw(Program& program) const noexcept
{
  program.use();
  this->vao_.bind();

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

  for (auto i = 0u; i < this->textures_.size(); ++i) {
    auto const& texture = this->textures_[i];
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

  if (!this->indices_.empty())
    this->ebo_.draw(this->topology_);
  else
    this->vbo_.draw(this->topology_);

  program.apply();

  for (auto const& texture : this->textures_)
    texture.unbind();
  this->vao_.unbind();
}

std::vector<Vertex> const& Mesh::getVertices() const noexcept
{
  return this->vertices_;
}
} // namespace redoom::graphics
