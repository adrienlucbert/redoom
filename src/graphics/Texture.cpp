#include <redoom/graphics/Texture.hh>

#include <fmt/core.h>
#include <stb/stb_image.h>

#include <redoom/graphics/TextureLibrary.hh>

namespace redoom::graphics
{
Texture2D::Texture2D(unsigned int pid,
    int pwidth,
    int pheight,
    int pchannels,
    Type ptype) noexcept
  : id{pid}
  , unit{0}
  , width{pwidth}
  , height{pheight}
  , channels{pchannels}
  , type{ptype}
{
}

Texture2D::Texture2D(Texture2D&& b) noexcept
  : id{b.id}
  , unit{0}
  , width{b.width}
  , height{b.height}
  , channels{b.channels}
  , type{b.type}
{
  b.id = 0;
}

Texture2D::~Texture2D() noexcept
{
  if (this->id != 0)
    glDeleteTextures(1, &this->id);
}

Texture2D& Texture2D::operator=(Texture2D&& rhs) noexcept
{
  if (this != &rhs) {
    std::swap(this->id, rhs.id);
    this->width = rhs.width;
    this->height = rhs.height;
    this->channels = rhs.channels;
  }
  return *this;
}

Expected<Texture2D> Texture2D::fromData(unsigned char const* data,
    int width,
    int height,
    int channels,
    Type type) noexcept
{
  GLenum format; // NOLINT
  if (channels == 1)
    format = GL_RED;
  else if (channels == 3)
    format = GL_RGB;
  else if (channels == 4)
    format = GL_RGBA;
  else {
    return make_formatted_unexpected(
        "Unsupported channels count: {}", channels);
  }
  unsigned int id; // NOLINT
  glGenTextures(1, &id);
  glBindTexture(GL_TEXTURE_2D, id);
  // TODO(alucbert): make the following options tweakable via an option
  // parameter
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
  glTexParameteri(
      GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
  glTexImage2D(GL_TEXTURE_2D,
      0,
      static_cast<GLint>(format),
      width,
      height,
      0,
      format,
      GL_UNSIGNED_BYTE,
      data);
  glGenerateMipmap(GL_TEXTURE_2D);
  glBindTexture(GL_TEXTURE_2D, 0);
  return Texture2D{id, width, height, channels, type};
}

Expected<Texture2D> Texture2D::fromFile(
    std::filesystem::path const& path, Type type) noexcept
{
  // Search textures cache
  auto texture_opt = TextureLibrary::getTexture(path);
  if (texture_opt.has_value())
    return *texture_opt;

  int width;    // NOLINT
  int height;   // NOLINT
  int channels; // NOLINT
  stbi_set_flip_vertically_on_load(1);
  unsigned char* data = stbi_load(path.c_str(), &width, &height, &channels, 0);
  if (data == nullptr)
    return tl::make_unexpected("Failed to load texture");
  else {
    auto texture = Texture2D::fromData(data, width, height, channels, type);
    stbi_image_free(data);
    if (texture.has_value())
      TextureLibrary::addTexture(path, *texture);
    return texture;
  }
}

unsigned int Texture2D::getId() const noexcept
{
  return this->id;
}

int Texture2D::getWidth() const noexcept
{
  return this->width;
}

int Texture2D::getHeight() const noexcept
{
  return this->height;
}

int Texture2D::getChannels() const noexcept
{
  return this->channels;
}

Texture2D::Type Texture2D::getType() const noexcept
{
  return this->type;
}

void Texture2D::setUnit(
    Program& program, std::string_view uniform, GLint punit) const noexcept
{
  this->unit = punit;
  program.use();
  glActiveTexture(GL_TEXTURE0 + this->unit);
  program.setUniform(uniform, this->unit);
}

void Texture2D::bind() const noexcept
{
  glActiveTexture(GL_TEXTURE0 + this->unit);
  glBindTexture(GL_TEXTURE_2D, this->id);
}

void Texture2D::unbind() const noexcept // NOLINT
{
  glBindTexture(GL_TEXTURE_2D, 0);
  glActiveTexture(GL_TEXTURE0);
}

Expected<Texture2D> Texture2D::getPlaceholder() noexcept
{
  unsigned char const data[] = {255, 255, 255, 255}; // NOLINT
  return Texture2D::fromData(data, 1, 1, 4, Type::Diffuse);
}
} // namespace redoom::graphics
