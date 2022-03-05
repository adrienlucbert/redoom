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
  Mesh(std::vector<Vertex> vertices,
      std::vector<unsigned int> indices,
      std::vector<Texture2D> textures,
      GLenum topology = GL_TRIANGLES) noexcept;
  Mesh(Mesh const& b) noexcept = delete;
  Mesh(Mesh&& b) noexcept = default;
  virtual ~Mesh() noexcept = default;

  Mesh& operator=(Mesh const& rhs) noexcept = delete;
  Mesh& operator=(Mesh&& rhs) noexcept = default;

  void draw(Program& program) const noexcept;

  [[nodiscard]] std::vector<Vertex> const& getVertices() const noexcept;

private:
  std::vector<Vertex> vertices_;
  std::vector<unsigned int> indices_;
  std::vector<Texture2D> textures_;

  GLenum topology_;
  VertexArray vao_;
  VertexBuffer<std::vector<Vertex>> vbo_;
  IndexBuffer<std::vector<unsigned int>> ebo_;
};
} // namespace redoom::graphics
