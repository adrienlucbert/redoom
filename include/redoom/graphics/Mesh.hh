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
  Mesh(
      std::vector<Vertex> vertices, std::vector<unsigned int> indices) noexcept;
  Mesh(Mesh const&) noexcept = delete;
  Mesh(Mesh&&) noexcept = default;
  virtual ~Mesh() noexcept = default;

  Mesh& operator=(Mesh const&) noexcept = delete;
  Mesh& operator=(Mesh&&) noexcept = default;

  void draw() const noexcept;

  [[nodiscard]] virtual std::string const& getType() const noexcept;
  [[nodiscard]] std::vector<Vertex> const& getVertices() const noexcept;
  [[nodiscard]] std::vector<unsigned int> const& getIndices() const noexcept;

protected:
  std::vector<Vertex> vertices_;
  std::vector<unsigned int> indices_;

  VertexArray vao_;
  VertexBuffer<std::vector<Vertex>> vbo_;
  IndexBuffer<std::vector<unsigned int>> ebo_;
};
} // namespace redoom::graphics
