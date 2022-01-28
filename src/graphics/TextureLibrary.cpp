#include <redoom/graphics/TextureLibrary.hh>

namespace redoom::graphics
{
// NOLINTNEXTLINE
auto TextureLibrary::textures = std::unordered_map<std::string, Texture2D>{};

void TextureLibrary::addTexture(
    std::filesystem::path const& path, Texture2D texture) noexcept
{
  TextureLibrary::textures.emplace(path, std::move(texture));
}

tl::optional<Texture2D&> TextureLibrary::getTexture(
    std::filesystem::path const& path) noexcept
{
  auto const& texture_it = TextureLibrary::textures.find(path);
  if (texture_it == TextureLibrary::textures.end())
    return tl::nullopt;
  else
    return texture_it->second;
}
} // namespace redoom::graphics
