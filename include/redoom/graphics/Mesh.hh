#pragma once

#include <GL/glew.h>
#include <fmt/format.h>

#include <Utils/Concepts.hpp>
#include <redoom/graphics/Buffer.hh>
#include <redoom/graphics/Camera.hh>
#include <redoom/graphics/Program.hh>
#include <redoom/graphics/Texture.hh>
#include <redoom/graphics/Vertex.hh>
#include <redoom/graphics/VertexArray.hh>

namespace redoom::graphics
{
template <concepts::Container<Vertex> VerticesContainer,
    concepts::Container<GLuint> IndicesContainer,
    concepts::Container<Texture2D> TexturesContainer>
class Mesh
{
public:
  Mesh(VerticesContainer pvertices,
      IndicesContainer pindices,
      TexturesContainer ptextures) noexcept
    : vertices{std::move(pvertices)}
    , indices{std::move(pindices)}
    , textures{std::move(ptextures)}
    , vao{}
    , vbo{this->vertices, BufferUsage::STATIC}
    , ebo{this->indices, BufferUsage::STATIC}
  {
    this->vao.bind();
    this->vbo.bind();
    this->ebo.bind();
    this->vbo.linkAttrib(0, 3, GL_FLOAT, 0);
    this->vbo.linkAttrib(1, 3, GL_FLOAT, 3 * sizeof(float));
    this->vbo.linkAttrib(2, 3, GL_FLOAT, 6 * sizeof(float));
    this->vbo.linkAttrib(3, 2, GL_FLOAT, 9 * sizeof(float));
    this->vao.unbind();
    this->vbo.unbind();
    this->ebo.unbind();
  }
  Mesh(Mesh const& b) noexcept = delete;
  Mesh(Mesh&& b) noexcept = default;
  ~Mesh() noexcept = default;

  Mesh& operator=(Mesh const& rhs) noexcept = delete;
  Mesh& operator=(Mesh&& rhs) noexcept = default;

  void draw(Program& program) const noexcept
  {
    program.use();
    this->vao.bind();

    for (auto i = 0u; i < this->textures.size(); ++i) {
      auto& texture = this->textures[i];
      auto const unit = static_cast<GLint>(i);
      auto const uniform = fmt::format("texture{}", unit);
      texture.setUnit(program, uniform, unit);
      texture.bind();
    }

    if (!this->indices.empty())
      this->ebo.draw();
    else
      this->vbo.draw();
  }

private:
  VerticesContainer vertices;
  IndicesContainer indices;
  TexturesContainer textures;
  VertexArray vao;
  VertexBuffer<VerticesContainer> vbo;
  IndexBuffer<IndicesContainer> ebo;
};
} // namespace redoom::graphics
