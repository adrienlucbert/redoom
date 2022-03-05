#pragma once

#include <vector>

#include <GL/glew.h>

#include <redoom/graphics/Texture.hh>
#include <redoom/graphics/Vertex.hh>
#include <redoom/graphics/mesh/Cuboid.hh>

namespace redoom::graphics::mesh
{
class Cube : public Cuboid
{
public:
  explicit Cube(float width,
      glm::vec3 color = {1.0f, 1.0f, 1.0f},
      std::vector<Texture2D> textures = {},
      GLenum topology = GL_TRIANGLES) noexcept
    : Cuboid(width, width, width, color, std::move(textures), topology)
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
