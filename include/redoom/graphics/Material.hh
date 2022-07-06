#include <string>

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
      float pmetallicness) noexcept;
  Material(Material const& b) noexcept = delete;
  Material(Material&& b) noexcept = default;
  virtual ~Material() noexcept = default;

  Material& operator=(Material const& rhs) noexcept = delete;
  Material& operator=(Material&& rhs) noexcept = default;

  void apply(Program& program) const noexcept;

  std::string shader;
  float gloss;
  float smoothness;
  float metallicness;
};
} // namespace redoom::graphics
