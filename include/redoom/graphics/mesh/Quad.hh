#include <GL/glew.h>

#include <redoom/graphics/Mesh.hh>
#include <redoom/graphics/Texture.hh>
#include <redoom/graphics/Vertex.hh>

namespace redoom::graphics::mesh
{
class Quad : public Mesh
{
public:
  explicit Quad(float width,
      float height,
      glm::vec3 color = {1.0f, 1.0f, 1.0f},
      std::vector<Texture2D> ptextures = {}) noexcept
    // source:
    // https://github.com/JoeyDeVries/Cell/blob/master/cell/mesh/quad.cpp
    // clang-format off
    : Mesh{
        std::vector{
          // TODO(alucbert): determine actual normals
          Vertex{{-width,  height, 0.0f}, {0.0f, 0.0f, 0.0f}, color, {0.0f, 1.0f}},
          Vertex{{-width, -height, 0.0f}, {0.0f, 0.0f, 0.0f}, color, {0.0f, 0.0f}},
          Vertex{{ width,  height, 0.0f}, {0.0f, 0.0f, 0.0f}, color, {1.0f, 1.0f}},
          Vertex{{ width, -height, 0.0f}, {0.0f, 0.0f, 0.0f}, color, {1.0f, 0.0f}},
        },
        std::vector<GLuint>{},
        std::move(ptextures),
        GL_TRIANGLE_STRIP
    } // clang-format on
  {
  }
  Quad(Quad const& b) noexcept = delete;
  Quad(Quad&& b) noexcept = default;
  ~Quad() noexcept override = default;

  Quad& operator=(Quad const& rhs) noexcept = delete;
  Quad& operator=(Quad&& rhs) noexcept = default;

private:
};
} // namespace redoom::graphics::mesh