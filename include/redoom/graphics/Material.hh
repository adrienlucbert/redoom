#include <string>

#include <glm/gtc/type_ptr.hpp>

#pragma once

namespace redoom::graphics
{
class Program;

class Material
{
public:
  explicit Material(std::string pshader,
      float pgloss,
      float psmoothness,
      float pmetallicness,
      glm::vec3 pcolor) noexcept;
  Material(Material const&) noexcept = delete;
  Material(Material&&) noexcept = default;
  virtual ~Material() noexcept = default;

  Material& operator=(Material const&) noexcept = delete;
  Material& operator=(Material&&) noexcept = default;

  void apply(Program& program) const noexcept;

  std::string shader;
  float gloss;
  float smoothness;
  float metallicness;
  glm::vec3 color;
};
} // namespace redoom::graphics
