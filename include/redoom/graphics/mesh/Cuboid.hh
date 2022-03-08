#pragma once

#include <vector>

#include <GL/glew.h>

#include <redoom/graphics/Mesh.hh>
#include <redoom/graphics/Texture.hh>
#include <redoom/graphics/Vertex.hh>

namespace redoom::graphics::mesh
{
class Cuboid : public Mesh
{
public:
  Cuboid(float width,
      float height,
      float depth,
      glm::vec3 color = {1.0f, 1.0f, 1.0f},
      std::vector<Texture2D> textures = {},
      GLenum topology = GL_TRIANGLES) noexcept
    // clang-format off
    : Mesh{
        std::vector{
          // front face
          Vertex{{-width / 2, -height / 2, -depth / 2}, {  0.0f,    0.0f, -depth}, color, {0.0f, 0.0f}},
          Vertex{{ width / 2,  height / 2, -depth / 2}, {  0.0f,    0.0f, -depth}, color, {1.0f, 1.0f}},
          Vertex{{ width / 2, -height / 2, -depth / 2}, {  0.0f,    0.0f, -depth}, color, {1.0f, 0.0f}},
          Vertex{{ width / 2,  height / 2, -depth / 2}, {  0.0f,    0.0f, -depth}, color, {1.0f, 1.0f}},
          Vertex{{-width / 2, -height / 2, -depth / 2}, {  0.0f,    0.0f, -depth}, color, {0.0f, 0.0f}},
          Vertex{{-width / 2,  height / 2, -depth / 2}, {  0.0f,    0.0f, -depth}, color, {0.0f, 1.0f}},

          // back face
          Vertex{{-width / 2, -height / 2,  depth / 2}, {  0.0f,    0.0f,  depth}, color, {0.0f, 0.0f}},
          Vertex{{ width / 2, -height / 2,  depth / 2}, {  0.0f,    0.0f,  depth}, color, {1.0f, 0.0f}},
          Vertex{{ width / 2,  height / 2,  depth / 2}, {  0.0f,    0.0f,  depth}, color, {1.0f, 1.0f}},
          Vertex{{ width / 2,  height / 2,  depth / 2}, {  0.0f,    0.0f,  depth}, color, {1.0f, 1.0f}},
          Vertex{{-width / 2,  height / 2,  depth / 2}, {  0.0f,    0.0f,  depth}, color, {0.0f, 1.0f}},
          Vertex{{-width / 2, -height / 2,  depth / 2}, {  0.0f,    0.0f,  depth}, color, {0.0f, 0.0f}},

          // left face
          Vertex{{-width / 2,  height / 2,  depth / 2}, {-width,    0.0f,   0.0f}, color, {1.0f, 0.0f}},
          Vertex{{-width / 2,  height / 2, -depth / 2}, {-width,    0.0f,   0.0f}, color, {1.0f, 1.0f}},
          Vertex{{-width / 2, -height / 2, -depth / 2}, {-width,    0.0f,   0.0f}, color, {0.0f, 1.0f}},
          Vertex{{-width / 2, -height / 2, -depth / 2}, {-width,    0.0f,   0.0f}, color, {0.0f, 1.0f}},
          Vertex{{-width / 2, -height / 2,  depth / 2}, {-width,    0.0f,   0.0f}, color, {0.0f, 0.0f}},
          Vertex{{-width / 2,  height / 2,  depth / 2}, {-width,    0.0f,   0.0f}, color, {1.0f, 0.0f}},

          // right face
          Vertex{{ width / 2,  height / 2,  depth / 2}, { width,    0.0f,   0.0f}, color, {1.0f, 0.0f}},
          Vertex{{ width / 2, -height / 2, -depth / 2}, { width,    0.0f,   0.0f}, color, {0.0f, 1.0f}},
          Vertex{{ width / 2,  height / 2, -depth / 2}, { width,    0.0f,   0.0f}, color, {1.0f, 1.0f}},
          Vertex{{ width / 2, -height / 2, -depth / 2}, { width,    0.0f,   0.0f}, color, {0.0f, 1.0f}},
          Vertex{{ width / 2,  height / 2,  depth / 2}, { width,    0.0f,   0.0f}, color, {1.0f, 0.0f}},
          Vertex{{ width / 2, -height / 2,  depth / 2}, { width,    0.0f,   0.0f}, color, {0.0f, 0.0f}},

          // bottom face
          Vertex{{-width / 2, -height / 2, -depth / 2}, {  0.0f, -height,   0.0f}, color, {0.0f, 1.0f}},
          Vertex{{ width / 2, -height / 2, -depth / 2}, {  0.0f, -height,   0.0f}, color, {1.0f, 1.0f}},
          Vertex{{ width / 2, -height / 2,  depth / 2}, {  0.0f, -height,   0.0f}, color, {1.0f, 0.0f}},
          Vertex{{ width / 2, -height / 2,  depth / 2}, {  0.0f, -height,   0.0f}, color, {1.0f, 0.0f}},
          Vertex{{-width / 2, -height / 2,  depth / 2}, {  0.0f, -height,   0.0f}, color, {0.0f, 0.0f}},
          Vertex{{-width / 2, -height / 2, -depth / 2}, {  0.0f, -height,   0.0f}, color, {0.0f, 1.0f}},

          // top face
          Vertex{{-width / 2,  height / 2, -depth / 2}, {  0.0f,  height,   0.0f}, color, {0.0f, 1.0f}},
          Vertex{{ width / 2,  height / 2,  depth / 2}, {  0.0f,  height,   0.0f}, color, {1.0f, 0.0f}},
          Vertex{{ width / 2,  height / 2, -depth / 2}, {  0.0f,  height,   0.0f}, color, {1.0f, 1.0f}},
          Vertex{{ width / 2,  height / 2,  depth / 2}, {  0.0f,  height,   0.0f}, color, {1.0f, 0.0f}},
          Vertex{{-width / 2,  height / 2, -depth / 2}, {  0.0f,  height,   0.0f}, color, {0.0f, 1.0f}},
          Vertex{{-width / 2,  height / 2,  depth / 2}, {  0.0f,  height,   0.0f}, color, {0.0f, 0.0f}},
        },
        std::vector<GLuint>{},
        std::move(textures),
        topology
    } // clang-format on
  {
  }
  Cuboid(Cuboid const& b) noexcept = delete;
  Cuboid(Cuboid&& b) noexcept = default;
  ~Cuboid() noexcept override = default;

  Cuboid& operator=(Cuboid const& rhs) noexcept = delete;
  Cuboid& operator=(Cuboid&& rhs) noexcept = default;

private:
};
} // namespace redoom::graphics::mesh
