#pragma once

#include <filesystem>

#include <GL/glew.h>

#include <Utils/Expected.hh>
#include <redoom/graphics/Program.hh>

namespace redoom::graphics
{
class Texture2D
{
public:
  enum class Type {
    Diffuse,
    Specular,
    Ambient,
    Emissive,
    Height,
    Normals,
    Shininess,
    Opacity,
    Displacement,
    Lightmap,
    Reflection,
    BaseColor
  };
  Texture2D(Texture2D const& b) noexcept = default;
  Texture2D(Texture2D&& b) noexcept;
  ~Texture2D() noexcept;

  Texture2D& operator=(Texture2D const& rhs) noexcept = delete;
  Texture2D& operator=(Texture2D&& rhs) noexcept;

  [[nodiscard]] static Expected<Texture2D> fromFile(
      std::filesystem::path const& path, Type type) noexcept;
  [[nodiscard]] static Expected<Texture2D> fromData(unsigned char const* data,
      int width,
      int height,
      int channels,
      Type type) noexcept;

  [[nodiscard]] unsigned int getId() const noexcept;
  [[nodiscard]] int getWidth() const noexcept;
  [[nodiscard]] int getHeight() const noexcept;
  [[nodiscard]] int getChannels() const noexcept;
  [[nodiscard]] Type getType() const noexcept;

  void setUnit(
      Program& program, std::string_view uniform, GLint unit) const noexcept;

  void bind() const noexcept;
  void unbind() const noexcept;

  static Expected<Texture2D> getPlaceholder() noexcept;

private:
  explicit Texture2D(
      unsigned int id, int width, int height, int channels, Type type) noexcept;

  unsigned int id_;
  mutable GLint unit_;
  int width_;
  int height_;
  int channels_;
  Type type_;
};
} // namespace redoom::graphics
