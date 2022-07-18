#include <string>
#include <vector>

#include <glm/gtc/type_ptr.hpp>

#include <redoom/graphics/Texture.hh>

#pragma once

namespace redoom::graphics
{
class Material
{
public:
  explicit Material(std::string shader,
      float gloss,
      float smoothness,
      float metallicness,
      glm::vec3 color,
      std::vector<std::reference_wrapper<Texture2D>> textures,
      unsigned int topology) noexcept;
  Material(Material const&) noexcept = delete;
  Material(Material&&) noexcept = default;
  virtual ~Material() noexcept = default;

  Material& operator=(Material const&) noexcept = delete;
  Material& operator=(Material&&) noexcept = default;

  void use() const noexcept;

  std::string shader_;
  float gloss_;
  float smoothness_;
  float metallicness_;
  glm::vec3 color_;
  std::vector<std::reference_wrapper<Texture2D>> textures_{};
  unsigned int topology_;
};
} // namespace redoom::graphics
