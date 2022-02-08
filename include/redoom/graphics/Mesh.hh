#pragma once

#include <GL/glew.h>

#include <redoom/graphics/Buffer.hh>
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
      GLenum ptopology = GL_TRIANGLES) noexcept;
  Mesh(Mesh const& b) noexcept = delete;
  Mesh(Mesh&& b) noexcept = default;
  virtual ~Mesh() noexcept = default;

  Mesh& operator=(Mesh const& rhs) noexcept = delete;
  Mesh& operator=(Mesh&& rhs) noexcept = default;

  void draw(Program& program) const noexcept;

  [[nodiscard]] std::vector<Vertex> const& getVertices() const noexcept;

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
