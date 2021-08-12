#include <numbers>
#include <utility>
#include <vector>

#include <GL/glew.h>

#include <Utils/Concepts.hpp>
#include <redoom/graphics/Mesh.hh>
#include <redoom/graphics/Texture.hh>
#include <redoom/graphics/Vertex.hh>

namespace redoom::graphics::mesh
{
template <concepts::Container<Texture2D> TexturesContainer>
class Sphere
  : public Mesh<std::vector<Vertex>, std::vector<GLuint>, TexturesContainer>
{
public:
  Sphere(unsigned int x_segments,
      unsigned int y_segments,
      glm::vec3 color = {1.0f, 1.0f, 1.0f},
      TexturesContainer ptextures = {}) noexcept
    : Mesh<std::vector<Vertex>, std::vector<GLuint>, TexturesContainer>{
        Sphere::create(x_segments, y_segments, color, std::move(ptextures))}
  {
  }
  Sphere(Sphere const& b) noexcept = delete;
  Sphere(Sphere&& b) noexcept = default;
  ~Sphere() noexcept override = default;

  Sphere& operator=(Sphere const& rhs) noexcept = delete;
  Sphere& operator=(Sphere&& rhs) noexcept = default;

private:
  static Mesh<std::vector<Vertex>, std::vector<GLuint>, TexturesContainer>
  create(unsigned int x_segments,
      unsigned int y_segments,
      glm::vec3 color,
      TexturesContainer ptextures = {}) noexcept
  {
    // source:
    // https://github.com/JoeyDeVries/Cell/blob/master/cell/mesh/sphere.cpp
    auto vertices = std::vector<Vertex>{};
    auto indices = std::vector<GLuint>{};

    for (auto y = 0u; y <= y_segments; ++y) {
      for (auto x = 0u; x <= x_segments; ++x) {
        auto x_segment =
            static_cast<double>(x) / static_cast<double>(x_segments);
        auto y_segment =
            static_cast<double>(y) / static_cast<double>(y_segments);
        auto x_pos = std::cos(x_segment * 2 * std::numbers::pi)
                   * std::sin(y_segment * std::numbers::pi); // TAU is 2PI
        auto y_pos = std::cos(y_segment * std::numbers::pi);
        auto z_pos = std::sin(x_segment * 2 * std::numbers::pi)
                   * std::sin(y_segment * std::numbers::pi);

        auto pos = glm::vec3(x_pos, y_pos, z_pos);
        auto normal = glm::vec3(x_pos, y_pos, z_pos);
        auto uv = glm::vec2(x_segment, y_segment);
        vertices.push_back({pos, normal, color, uv});
      }
    }

    for (auto y = 0u; y < y_segments; ++y) {
      for (auto x = 0u; x < x_segments; ++x) {
        indices.push_back((y + 1) * (x_segments + 1) + x);
        indices.push_back(y * (x_segments + 1) + x);
        indices.push_back(y * (x_segments + 1) + x + 1);

        indices.push_back((y + 1) * (x_segments + 1) + x);
        indices.push_back(y * (x_segments + 1) + x + 1);
        indices.push_back((y + 1) * (x_segments + 1) + x + 1);
      }
    }

    return Mesh{std::move(vertices),
        std::move(indices),
        std::move(ptextures),
        GL_TRIANGLES};
  }
};
} // namespace redoom::graphics::mesh
