#pragma once

#include <tl/optional.hpp>

#include <Utils/Expected.hh>
#include <redoom/graphics/Texture.hh>

namespace redoom::graphics
{
class TextureLibrary
{
public:
  static void addTexture(
      std::filesystem::path const& path, Texture2D texture) noexcept;
  static tl::optional<Texture2D&> getTexture(
      std::filesystem::path const& path) noexcept;

private:
  static std::unordered_map<std::string, Texture2D> textures_; // NOLINT
};
} // namespace redoom::graphics
