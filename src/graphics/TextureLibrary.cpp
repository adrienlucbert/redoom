#include <redoom/graphics/TextureLibrary.hh>

namespace redoom::graphics
{
// NOLINTNEXTLINE
auto TextureLibrary::textures_ = std::unordered_map<std::string, Texture2D>{};

Texture2D& TextureLibrary::addTexture(
    std::string path, Texture2D texture) noexcept
{
  auto it =
      TextureLibrary::textures_.emplace(std::move(path), std::move(texture));
  return it.first->second;
}

tl::optional<Texture2D&> TextureLibrary::getTexture(
    std::filesystem::path const& path) noexcept
{
  auto const& texture_it = TextureLibrary::textures_.find(path);
  if (texture_it == TextureLibrary::textures_.end())
    return tl::nullopt;
  else
    return texture_it->second;
}
} // namespace redoom::graphics
