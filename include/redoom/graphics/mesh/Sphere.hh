#pragma once

#include <numbers>
#include <utility>
#include <vector>

#include <GL/glew.h>

#include <redoom/graphics/Mesh.hh>
#include <redoom/graphics/Texture.hh>
#include <redoom/graphics/Vertex.hh>

namespace redoom::graphics::mesh
{
class Sphere : public Mesh
{
public:
  explicit Sphere(float radius,
      unsigned int x_segment = 20,
      unsigned int y_segment = 20,
      std::vector<Texture2D> textures = {},
      GLenum topology = GL_TRIANGLES) noexcept
    : Mesh{Sphere::create(
        radius, x_segment, y_segment, std::move(textures), topology)}
    , radius_{radius}
    , x_segment_{x_segment}
    , y_segment_{y_segment}
  {
  }
  Sphere(Sphere const&) noexcept = delete;
  Sphere(Sphere&&) noexcept = default;
  ~Sphere() noexcept override = default;

  Sphere& operator=(Sphere const&) noexcept = delete;
  Sphere& operator=(Sphere&&) noexcept = default;

  [[nodiscard]] std::string const& getType() const noexcept override
  {
    static auto const type = std::string{"Sphere"};
    return type;
  }

  float radius_;
  unsigned int x_segment_;
  unsigned int y_segment_;

private:
  static Mesh create(float radius,
      unsigned int x_segment,
      unsigned int y_segment,
      std::vector<Texture2D> textures,
      GLenum topology) noexcept
  {
    // source:
    // http://www.songho.ca/opengl/gl_sphere.html
    auto vertices = std::vector<Vertex>{};
    auto indices = std::vector<GLuint>{};

    auto length_inv = 1.0f / radius;
    auto x_step = 2 * std::numbers::pi / x_segment;
    auto y_step = std::numbers::pi / x_segment;

    for (auto i = 0u; i <= y_segment; ++i) {
      auto y_angle = std::numbers::pi / 2 - i * y_step;
      auto xy = radius * static_cast<float>(std::cos(y_angle));
      auto z = radius * static_cast<float>(std::sin(y_angle));

      for (auto j = 0u; j <= x_segment; ++j) {
        auto x_angle = j * x_step;

        auto x = xy * static_cast<float>(std::cos(x_angle));
        auto y = xy * static_cast<float>(std::sin(x_angle));
        auto pos = glm::vec3{x, y, z};

        auto nx = x * length_inv;
        auto ny = y * length_inv;
        auto nz = z * length_inv;
        auto norm = glm::vec3{nx, ny, nz};

        auto s = static_cast<float>(i) / static_cast<float>(x_segment);
        auto t = static_cast<float>(j) / static_cast<float>(y_segment);
        auto uv = glm::vec2{s, t};

        vertices.push_back({pos, norm, uv});
      }
    }

    for (auto i = 0u; i < y_segment; ++i) {
      auto k1 = i * (x_segment + 1);
      auto k2 = k1 + x_segment + 1;

      for (auto j = 0u; j < x_segment; ++j, ++k1, ++k2) {
        if (i != 0) {
          indices.push_back(k1);
          indices.push_back(k2);
          indices.push_back(k1 + 1);
        }

        if (i != (y_segment - 1)) {
          indices.push_back(k1 + 1);
          indices.push_back(k2);
          indices.push_back(k2 + 1);
        }
      }
    }

    return Mesh{
        std::move(vertices), std::move(indices), std::move(textures), topology};
  }
};
} // namespace redoom::graphics::mesh
