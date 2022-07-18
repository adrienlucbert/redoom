#pragma once

#include <GL/glew.h>

#include <redoom/graphics/Mesh.hh>
#include <redoom/graphics/Texture.hh>
#include <redoom/graphics/Vertex.hh>

namespace redoom::graphics::mesh
{
class Quad : public Mesh
{
public:
  explicit Quad(float width, float height) noexcept
    // source:
    // https://github.com/JoeyDeVries/Cell/blob/master/cell/mesh/quad.cpp
    // clang-format off
    : Mesh{
        std::vector{
          // TODO(alucbert): determine actual normals
          Vertex{{-width * 0.5f,  height * 0.5f, 0.0f}, {0.0f, 0.0f, 0.0f}, {0.0f, 1.0f}},
          Vertex{{-width * 0.5f, -height * 0.5f, 0.0f}, {0.0f, 0.0f, 0.0f}, {0.0f, 0.0f}},
          Vertex{{ width * 0.5f,  height * 0.5f, 0.0f}, {0.0f, 0.0f, 0.0f}, {1.0f, 1.0f}},
          Vertex{{ width * 0.5f, -height * 0.5f, 0.0f}, {0.0f, 0.0f, 0.0f}, {1.0f, 0.0f}},
        },
        std::vector<GLuint>{0, 1, 3, 0, 3, 2}
      } // clang-format on
    , size_{width, height}
  {
  }
  Quad(Quad const&) noexcept = delete;
  Quad(Quad&&) noexcept = default;
  ~Quad() noexcept override = default;

  Quad& operator=(Quad const&) noexcept = delete;
  Quad& operator=(Quad&&) noexcept = default;

  [[nodiscard]] std::string const& getType() const noexcept override
  {
    static auto const type = std::string{"Quad"};
    return type;
  }

  glm::vec2 size_;
};
} // namespace redoom::graphics::mesh
