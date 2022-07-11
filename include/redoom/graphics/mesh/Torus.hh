#pragma once

#include <numbers>
#include <utility>
#include <vector>

#include <GL/glew.h>
#include <yaml-cpp/emitter.h>

#include <redoom/graphics/Mesh.hh>
#include <redoom/graphics/Texture.hh>
#include <redoom/graphics/Vertex.hh>

namespace redoom::graphics::mesh
{
class Torus : public Mesh
{
public:
  Torus(float r1,
      float r2,
      unsigned int num_steps1,
      unsigned int num_steps2,
      std::vector<Texture2D> textures = {},
      GLenum topology = GL_TRIANGLES) noexcept
    : Mesh{Torus::create(
        r1, r2, num_steps1, num_steps2, std::move(textures), topology)}
    , r1_{r1}
    , r2_{r2}
    , num_steps1_{num_steps1}
    , num_steps2_{num_steps2}
  {
  }
  Torus(Torus const&) noexcept = delete;
  Torus(Torus&&) noexcept = default;
  ~Torus() noexcept override = default;

  Torus& operator=(Torus const&) noexcept = delete;
  Torus& operator=(Torus&&) noexcept = default;

  [[nodiscard]] std::string const& getType() const noexcept override
  {
    static auto const type = std::string{"Torus"};
    return type;
  }

  float r1_;
  float r2_;
  unsigned int num_steps1_;
  unsigned int num_steps2_;

private:
  static Mesh create(float r1,
      float r2,
      unsigned int num_steps1,
      unsigned int num_steps2,
      std::vector<Texture2D> textures,
      GLenum topology) noexcept
  {
    // source:
    // https://github.com/JoeyDeVries/Cell/blob/master/cell/mesh/sphere.cpp
    auto vertices = std::vector<Vertex>{};
    auto indices = std::vector<GLuint>{};

    vertices.resize((num_steps1 + 1) * (num_steps2 + 1));

    std::vector<glm::vec3> p(num_steps1 + 1);
    auto a = 0.0f;
    auto step = 2.0f * static_cast<float>(std::numbers::pi)
              / static_cast<float>(num_steps1);
    for (auto i = 0u; i <= num_steps1; ++i) {
      auto x = std::cos(a) * r1;
      auto y = std::sin(a) * r1;
      p[i] = glm::vec3{x, y, 0.0f};
      a += step;
    }

    for (auto i = 0u; i <= num_steps1; ++i) {
      glm::vec3 u = glm::normalize(glm::vec3(0.0f) - p[i]) * r2;
      glm::vec3 v = glm::vec3(0.0f, 0.0f, 1.0f) * r2;

      auto _a = 0.0f;
      auto _step = 2.0f * static_cast<float>(std::numbers::pi)
                 / static_cast<float>(num_steps2);
      for (auto j = 0u; j <= num_steps2; ++j) {
        auto c = std::cos(_a);
        auto s = std::sin(_a);

        auto pos = p[i] + c * u + s * v;
        auto normal = glm::normalize(c * u + s * v);
        auto uv =
            glm::vec2{static_cast<float>(i) / static_cast<float>(num_steps1) * 2
                          * static_cast<float>(std::numbers::pi),
                static_cast<float>(j) / static_cast<float>(num_steps2)};
        vertices[i * (num_steps2 + 1) + j] = Vertex{pos, normal, uv};
        _a += _step;
      }
    }

    indices.resize(num_steps1 * num_steps2 * 6);

    auto index = 0u;
    for (auto i = 0u; i < num_steps1; ++i) {
      auto i1 = i;
      auto i2 = (i1 + 1);

      for (auto j = 0u; j < num_steps2; ++j) {
        auto j1 = j;
        auto j2 = (j1 + 1);

        indices[index++] = i1 * (num_steps2 + 1) + j1;
        indices[index++] = i1 * (num_steps2 + 1) + j2;
        indices[index++] = i2 * (num_steps2 + 1) + j1;

        indices[index++] = i2 * (num_steps2 + 1) + j2;
        indices[index++] = i2 * (num_steps2 + 1) + j1;
        indices[index++] = i1 * (num_steps2 + 1) + j2;
      }
    }

    return Mesh{
        std::move(vertices), std::move(indices), std::move(textures), topology};
  }
};
} // namespace redoom::graphics::mesh
