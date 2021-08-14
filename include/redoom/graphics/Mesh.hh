#pragma once

#include <GL/glew.h>
#include <fmt/format.h>

#include <redoom/graphics/Buffer.hh>
#include <redoom/graphics/Camera.hh>
#include <redoom/graphics/Program.hh>
#include <redoom/graphics/Texture.hh>
#include <redoom/graphics/Vertex.hh>
#include <redoom/graphics/VertexArray.hh>

namespace redoom::graphics
{
class Mesh
{
public:
  Mesh(std::vector<Vertex> pvertices,
      std::vector<unsigned int> pindices,
      std::vector<Texture2D> ptextures,
      GLenum ptopology = GL_TRIANGLES) noexcept
    : vertices{std::move(pvertices)}
    , indices{std::move(pindices)}
    , textures{std::move(ptextures)}
    , topology{ptopology}
    , vao{}
    , vbo{this->vertices, BufferUsage::STATIC}
    , ebo{this->indices, BufferUsage::STATIC}
  {
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
  Mesh(Mesh const& b) noexcept = delete;
  Mesh(Mesh&& b) noexcept = default;
  virtual ~Mesh() noexcept = default;

  Mesh& operator=(Mesh const& rhs) noexcept = delete;
  Mesh& operator=(Mesh&& rhs) noexcept = default;

  void draw(Program& program) const noexcept
  {
    program.use();
    this->vao.bind();

    for (auto i = 0u; i < this->textures.size(); ++i) {
      const auto& texture = this->textures[i];
      auto const unit = static_cast<GLint>(i);
      auto const uniform = fmt::format("texture{}", unit);
      texture.setUnit(program, uniform, unit);
      texture.bind();
    }

    if (!this->indices.empty())
      this->ebo.draw(this->topology);
    else
      this->vbo.draw(this->topology);
  }

private:
  std::vector<Vertex> vertices;
  std::vector<unsigned int> indices;
  std::vector<Texture2D> textures;

  GLenum topology;
  VertexArray vao;
  VertexBuffer<std::vector<Vertex>> vbo;
  IndexBuffer<std::vector<unsigned int>> ebo;
};
} // namespace redoom::graphics
