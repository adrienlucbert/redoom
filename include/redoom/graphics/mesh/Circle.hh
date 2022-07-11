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
class Circle : public Mesh
{
public:
  Circle(unsigned int edge_segments,
      unsigned int ring_segments,
      std::vector<Texture2D> textures = {},
      GLenum topology = GL_TRIANGLE_STRIP) noexcept
    : Mesh{Circle::create(
        edge_segments, ring_segments, std::move(textures), topology)}
    , edge_segments_{edge_segments}
    , ring_segments_{ring_segments}
  {
  }
  Circle(Circle const&) noexcept = delete;
  Circle(Circle&&) noexcept = default;
  ~Circle() noexcept override = default;

  Circle& operator=(Circle const&) noexcept = delete;
  Circle& operator=(Circle&&) noexcept = default;

  [[nodiscard]] std::string const& getType() const noexcept override
  {
    static auto const type = std::string{"Circle"};
    return type;
  }

  unsigned int edge_segments_;
  unsigned int ring_segments_;

private:
  static Mesh create(unsigned int edge_segments,
      unsigned int ring_segments,
      std::vector<Texture2D> textures,
      GLenum topology) noexcept
  {
    // source:
    // https://github.com/JoeyDeVries/Cell/blob/master/cell/mesh/circle.cpp
    auto vertices = std::vector<Vertex>{};
    auto indices = std::vector<GLuint>{};

    for (auto y = 0u; y <= ring_segments; ++y) {
      for (auto x = 0u; x <= edge_segments; ++x) {
        auto x_segment =
            static_cast<float>(x) / static_cast<float>(edge_segments);
        auto ring_depth =
            static_cast<float>(y) / static_cast<float>(ring_segments);
        float x_pos =
            std::cos(x_segment * 2 * static_cast<float>(std::numbers::pi));
        float y_pos =
            std::sin(x_segment * 2 * static_cast<float>(std::numbers::pi));

        // TODO(alucbert): determine actual normals and texture coordinates
        vertices.push_back({{x_pos * ring_depth, y_pos * ring_depth, 0.0f},
            {0.0f, 0.0f, 0.0f},
            {0.0f, 0.0f}});
      }
    }

    bool odd_row = false;
    for (auto y = 0u; y < ring_segments; ++y) {
      if (!odd_row) {
        for (auto x = 0u; x <= edge_segments; ++x) {
          indices.push_back(y * (edge_segments + 1) + x);
          indices.push_back((y + 1) * (edge_segments + 1) + x);
        }
      } else {
        for (int x = static_cast<int>(edge_segments); x >= 0; --x) {
          indices.push_back(
              (y + 1) * (edge_segments + 1) + static_cast<unsigned int>(x));
          indices.push_back(
              y * (edge_segments + 1) + static_cast<unsigned int>(x));
        }
      }
      odd_row = !odd_row;
    }

    return Mesh{
        std::move(vertices), std::move(indices), std::move(textures), topology};
  }
};
} // namespace redoom::graphics::mesh
