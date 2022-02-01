#include <redoom/graphics/Mesh.hh>

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
  , vao{}
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

  auto diffuse_id = 1u;
  auto specular_id = 1u;
  auto ambient_id = 1u;
  auto emissive_id = 1u;
  auto height_id = 1u;
  auto normals_id = 1u;
  auto shininess_id = 1u;
  auto opacity_id = 1u;
  auto displacement_id = 1u;
  auto lightmap_id = 1u;
  auto reflection_id = 1u;
  auto basecolor_id = 1u;

  for (auto i = 0u; i < this->textures.size(); ++i) {
    auto const& texture = this->textures[i];
    auto const unit = static_cast<GLint>(i + 1);
    auto uniform = std::string{};
    switch (texture.getType()) {
      case Texture2D::Type::Diffuse:
        uniform = fmt::format("texture_diffuse{}", diffuse_id++);
        break;
      case Texture2D::Type::Specular:
        uniform = fmt::format("texture_specular{}", specular_id++);
        break;
      case Texture2D::Type::Ambient:
        uniform = fmt::format("texture_ambient{}", ambient_id++);
        break;
      case Texture2D::Type::Emissive:
        uniform = fmt::format("texture_emissive{}", emissive_id++);
        break;
      case Texture2D::Type::Height:
        uniform = fmt::format("texture_height{}", height_id++);
        break;
      case Texture2D::Type::Normals:
        uniform = fmt::format("texture_normals{}", normals_id++);
        break;
      case Texture2D::Type::Shininess:
        uniform = fmt::format("texture_shininess{}", shininess_id++);
        break;
      case Texture2D::Type::Opacity:
        uniform = fmt::format("texture_opacity{}", opacity_id++);
        break;
      case Texture2D::Type::Displacement:
        uniform = fmt::format("texture_displacement{}", displacement_id++);
        break;
      case Texture2D::Type::Lightmap:
        uniform = fmt::format("texture_lightmap{}", lightmap_id++);
        break;
      case Texture2D::Type::Reflection:
        uniform = fmt::format("texture_reflection{}", reflection_id++);
        break;
      case Texture2D::Type::BaseColor:
        uniform = fmt::format("texture_base_color{}", basecolor_id++);
        break;
    }
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
} // namespace redoom::graphics
