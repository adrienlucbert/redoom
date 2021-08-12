#include <vector>

#include <GL/glew.h>

#include <redoom/graphics/Mesh.hh>
#include <redoom/graphics/Texture.hh>
#include <redoom/graphics/Vertex.hh>

namespace redoom::graphics::mesh
{
template <concepts::Container<Texture2D> TexturesContainer>
class Cube
  : public Mesh<std::vector<Vertex>, std::vector<GLuint>, TexturesContainer>
{
public:
  explicit Cube(glm::vec3 color = {1.0f, 1.0f, 1.0f},
      TexturesContainer ptextures = {}) noexcept
    // source:
    // https://github.com/JoeyDeVries/Cell/blob/master/cell/mesh/sphere.cpp
    // clang-format off
    : Mesh<std::vector<Vertex>, std::vector<GLuint>, TexturesContainer>{
        std::vector{
          Vertex{{-0.5f, -0.5f, -0.5f}, { 0.0f,  0.0f, -1.0f}, color, {0.0f, 0.0f}},
          Vertex{{0.5f, 0.5f, -0.5f},   { 0.0f,  0.0f, -1.0f}, color, {1.0f, 1.0f}},
          Vertex{{0.5f, -0.5f, -0.5f},  { 0.0f,  0.0f, -1.0f}, color, {1.0f, 0.0f}},
          Vertex{{0.5f, 0.5f, -0.5f},   { 0.0f,  0.0f, -1.0f}, color, {1.0f, 1.0f}},
          Vertex{{-0.5f, -0.5f, -0.5f}, { 0.0f,  0.0f, -1.0f}, color, {0.0f, 0.0f}},
          Vertex{{-0.5f, 0.5f, -0.5f},  { 0.0f,  0.0f, -1.0f}, color, {0.0f, 1.0f}},

          Vertex{{-0.5f, -0.5f, 0.5f},  { 0.0f,  0.0f,  1.0f}, color, {0.0f, 0.0f}},
          Vertex{{0.5f, -0.5f, 0.5f},   { 0.0f,  0.0f,  1.0f}, color, {1.0f, 0.0f}},
          Vertex{{0.5f, 0.5f, 0.5f},    { 0.0f,  0.0f,  1.0f}, color, {1.0f, 1.0f}},
          Vertex{{0.5f, 0.5f, 0.5f},    { 0.0f,  0.0f,  1.0f}, color, {1.0f, 1.0f}},
          Vertex{{-0.5f, 0.5f, 0.5f},   { 0.0f,  0.0f,  1.0f}, color, {0.0f, 1.0f}},
          Vertex{{-0.5f, -0.5f, 0.5f},  { 0.0f,  0.0f,  1.0f}, color, {0.0f, 0.0f}},

          Vertex{{-0.5f, 0.5f, 0.5f},   {-1.0f,  0.0f,  0.0f}, color, {1.0f, 0.0f}},
          Vertex{{-0.5f, 0.5f, -0.5f},  {-1.0f,  0.0f,  0.0f}, color, {1.0f, 1.0f}},
          Vertex{{-0.5f, -0.5f, -0.5f}, {-1.0f,  0.0f,  0.0f}, color, {0.0f, 1.0f}},
          Vertex{{-0.5f, -0.5f, -0.5f}, {-1.0f,  0.0f,  0.0f}, color, {0.0f, 1.0f}},
          Vertex{{-0.5f, -0.5f, 0.5f},  {-1.0f,  0.0f,  0.0f}, color, {0.0f, 0.0f}},
          Vertex{{-0.5f, 0.5f, 0.5f},   {-1.0f,  0.0f,  0.0f}, color, {1.0f, 0.0f}},

          Vertex{{0.5f, 0.5f, 0.5f},    { 1.0f,  0.0f,  0.0f}, color, {1.0f, 0.0f}},
          Vertex{{0.5f, -0.5f, -0.5f},  { 1.0f,  0.0f,  0.0f}, color, {0.0f, 1.0f}},
          Vertex{{0.5f, 0.5f, -0.5f},   { 1.0f,  0.0f,  0.0f}, color, {1.0f, 1.0f}},
          Vertex{{0.5f, -0.5f, -0.5f},  { 1.0f,  0.0f,  0.0f}, color, {0.0f, 1.0f}},
          Vertex{{0.5f, 0.5f, 0.5f},    { 1.0f,  0.0f,  0.0f}, color, {1.0f, 0.0f}},
          Vertex{{0.5f, -0.5f, 0.5f},   { 1.0f,  0.0f,  0.0f}, color, {0.0f, 0.0f}},

          Vertex{{-0.5f, -0.5f, -0.5f}, { 0.0f, -1.0f,  0.0f}, color, {0.0f, 1.0f}},
          Vertex{{0.5f, -0.5f, -0.5f},  { 0.0f, -1.0f,  0.0f}, color, {1.0f, 1.0f}},
          Vertex{{0.5f, -0.5f, 0.5f},   { 0.0f, -1.0f,  0.0f}, color, {1.0f, 0.0f}},
          Vertex{{0.5f, -0.5f, 0.5f},   { 0.0f, -1.0f,  0.0f}, color, {1.0f, 0.0f}},
          Vertex{{-0.5f, -0.5f, 0.5f},  { 0.0f, -1.0f,  0.0f}, color, {0.0f, 0.0f}},
          Vertex{{-0.5f, -0.5f, -0.5f}, { 0.0f, -1.0f,  0.0f}, color, {0.0f, 1.0f}},

          Vertex{{-0.5f, 0.5f, -0.5f},  { 0.0f,  1.0f,  0.0f}, color, {0.0f, 1.0f}},
          Vertex{{0.5f, 0.5f, 0.5f},    { 0.0f,  1.0f,  0.0f}, color, {1.0f, 0.0f}},
          Vertex{{0.5f, 0.5f, -0.5f},   { 0.0f,  1.0f,  0.0f}, color, {1.0f, 1.0f}},
          Vertex{{0.5f, 0.5f, 0.5f},    { 0.0f,  1.0f,  0.0f}, color, {1.0f, 0.0f}},
          Vertex{{-0.5f, 0.5f, -0.5f},  { 0.0f,  1.0f,  0.0f}, color, {0.0f, 1.0f}},
          Vertex{{-0.5f, 0.5f, 0.5f},   { 0.0f,  1.0f,  0.0f}, color, {0.0f, 0.0f}},
        },
        std::vector<GLuint>{},
        std::move(ptextures),
        GL_TRIANGLES
    } // clang-format on
  {
  }
  Cube(Cube const& b) noexcept = delete;
  Cube(Cube&& b) noexcept = default;
  ~Cube() noexcept override = default;

  Cube& operator=(Cube const& rhs) noexcept = delete;
  Cube& operator=(Cube&& rhs) noexcept = default;

private:
};
} // namespace redoom::graphics::mesh
