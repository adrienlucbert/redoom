#include <meshes/extern.hh>

#include <vector>

#include <GL/glew.h>
#include <yaml-cpp/yaml.h>

#include <redoom/graphics/Texture.hh>
#include <redoom/graphics/Vertex.hh>
#include <redoom/graphics/mesh/Cuboid.hh>

namespace redoom::graphics::mesh
{
class Cube : public Cuboid
{
public:
  explicit Cube(float width,
      std::vector<Texture2D> textures = {},
      GLenum topology = GL_TRIANGLES) noexcept
    : Cuboid(width, width, width, std::move(textures), topology)
  {
  }
  Cube(Cube const&) noexcept = delete;
  Cube(Cube&&) noexcept = default;
  ~Cube() noexcept override = default;

  Cube& operator=(Cube const&) noexcept = delete;
  Cube& operator=(Cube&&) noexcept = default;

  [[nodiscard]] std::string const& getType() const noexcept override
  {
    static auto const type = std::string{"Cube"};
    return type;
  }
};
} // namespace redoom::graphics::mesh
