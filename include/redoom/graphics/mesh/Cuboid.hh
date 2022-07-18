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
  Cuboid(float width, float height, float depth) noexcept
    // clang-format off
    : Mesh{
        std::vector{
          // front face
          Vertex{{-width / 2, -height / 2, -depth / 2}, {  0.0f,    0.0f, -depth}, {0.0f, 0.0f}},
          Vertex{{ width / 2,  height / 2, -depth / 2}, {  0.0f,    0.0f, -depth}, {1.0f, 1.0f}},
          Vertex{{ width / 2, -height / 2, -depth / 2}, {  0.0f,    0.0f, -depth}, {1.0f, 0.0f}},
          Vertex{{ width / 2,  height / 2, -depth / 2}, {  0.0f,    0.0f, -depth}, {1.0f, 1.0f}},
          Vertex{{-width / 2, -height / 2, -depth / 2}, {  0.0f,    0.0f, -depth}, {0.0f, 0.0f}},
          Vertex{{-width / 2,  height / 2, -depth / 2}, {  0.0f,    0.0f, -depth}, {0.0f, 1.0f}},

          // back face
          Vertex{{-width / 2, -height / 2,  depth / 2}, {  0.0f,    0.0f,  depth}, {0.0f, 0.0f}},
          Vertex{{ width / 2, -height / 2,  depth / 2}, {  0.0f,    0.0f,  depth}, {1.0f, 0.0f}},
          Vertex{{ width / 2,  height / 2,  depth / 2}, {  0.0f,    0.0f,  depth}, {1.0f, 1.0f}},
          Vertex{{ width / 2,  height / 2,  depth / 2}, {  0.0f,    0.0f,  depth}, {1.0f, 1.0f}},
          Vertex{{-width / 2,  height / 2,  depth / 2}, {  0.0f,    0.0f,  depth}, {0.0f, 1.0f}},
          Vertex{{-width / 2, -height / 2,  depth / 2}, {  0.0f,    0.0f,  depth}, {0.0f, 0.0f}},

          // left face
          Vertex{{-width / 2,  height / 2,  depth / 2}, {-width,    0.0f,   0.0f}, {1.0f, 0.0f}},
          Vertex{{-width / 2,  height / 2, -depth / 2}, {-width,    0.0f,   0.0f}, {1.0f, 1.0f}},
          Vertex{{-width / 2, -height / 2, -depth / 2}, {-width,    0.0f,   0.0f}, {0.0f, 1.0f}},
          Vertex{{-width / 2, -height / 2, -depth / 2}, {-width,    0.0f,   0.0f}, {0.0f, 1.0f}},
          Vertex{{-width / 2, -height / 2,  depth / 2}, {-width,    0.0f,   0.0f}, {0.0f, 0.0f}},
          Vertex{{-width / 2,  height / 2,  depth / 2}, {-width,    0.0f,   0.0f}, {1.0f, 0.0f}},

          // right face
          Vertex{{ width / 2,  height / 2,  depth / 2}, { width,    0.0f,   0.0f}, {1.0f, 0.0f}},
          Vertex{{ width / 2, -height / 2, -depth / 2}, { width,    0.0f,   0.0f}, {0.0f, 1.0f}},
          Vertex{{ width / 2,  height / 2, -depth / 2}, { width,    0.0f,   0.0f}, {1.0f, 1.0f}},
          Vertex{{ width / 2, -height / 2, -depth / 2}, { width,    0.0f,   0.0f}, {0.0f, 1.0f}},
          Vertex{{ width / 2,  height / 2,  depth / 2}, { width,    0.0f,   0.0f}, {1.0f, 0.0f}},
          Vertex{{ width / 2, -height / 2,  depth / 2}, { width,    0.0f,   0.0f}, {0.0f, 0.0f}},

          // bottom face
          Vertex{{-width / 2, -height / 2, -depth / 2}, {  0.0f, -height,   0.0f}, {0.0f, 1.0f}},
          Vertex{{ width / 2, -height / 2, -depth / 2}, {  0.0f, -height,   0.0f}, {1.0f, 1.0f}},
          Vertex{{ width / 2, -height / 2,  depth / 2}, {  0.0f, -height,   0.0f}, {1.0f, 0.0f}},
          Vertex{{ width / 2, -height / 2,  depth / 2}, {  0.0f, -height,   0.0f}, {1.0f, 0.0f}},
          Vertex{{-width / 2, -height / 2,  depth / 2}, {  0.0f, -height,   0.0f}, {0.0f, 0.0f}},
          Vertex{{-width / 2, -height / 2, -depth / 2}, {  0.0f, -height,   0.0f}, {0.0f, 1.0f}},

          // top face
          Vertex{{-width / 2,  height / 2, -depth / 2}, {  0.0f,  height,   0.0f}, {0.0f, 1.0f}},
          Vertex{{ width / 2,  height / 2,  depth / 2}, {  0.0f,  height,   0.0f}, {1.0f, 0.0f}},
          Vertex{{ width / 2,  height / 2, -depth / 2}, {  0.0f,  height,   0.0f}, {1.0f, 1.0f}},
          Vertex{{ width / 2,  height / 2,  depth / 2}, {  0.0f,  height,   0.0f}, {1.0f, 0.0f}},
          Vertex{{-width / 2,  height / 2, -depth / 2}, {  0.0f,  height,   0.0f}, {0.0f, 1.0f}},
          Vertex{{-width / 2,  height / 2,  depth / 2}, {  0.0f,  height,   0.0f}, {0.0f, 0.0f}},
        },
        std::vector<GLuint>{}
    } // clang-format on
    , size_{width, height, depth}
  {
  }
  Cuboid(Cuboid const&) noexcept = delete;
  Cuboid(Cuboid&&) noexcept = default;
  ~Cuboid() noexcept override = default;

  Cuboid& operator=(Cuboid const&) noexcept = delete;
  Cuboid& operator=(Cuboid&&) noexcept = default;

  [[nodiscard]] std::string const& getType() const noexcept override
  {
    static auto const type = std::string{"Cuboid"};
    return type;
  }

  glm::vec3 size_;
};
} // namespace redoom::graphics::mesh
