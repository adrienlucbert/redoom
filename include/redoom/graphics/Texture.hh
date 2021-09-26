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
  Texture2D(Texture2D const& b) noexcept = delete;
  Texture2D(Texture2D&& b) noexcept;
  ~Texture2D() noexcept;

  Texture2D& operator=(Texture2D const& rhs) noexcept = delete;
  Texture2D& operator=(Texture2D&& rhs) noexcept;

  [[nodiscard]] static Expected<Texture2D> fromFile(
      std::filesystem::path const& path) noexcept;
  [[nodiscard]] static Expected<Texture2D> fromData(
      unsigned char const* data, int width, int height, int pchannels) noexcept;

  [[nodiscard]] unsigned int getId() const noexcept;
  [[nodiscard]] int getWidth() const noexcept;
  [[nodiscard]] int getHeight() const noexcept;
  [[nodiscard]] int getChannels() const noexcept;

  void setUnit(
      Program& program, std::string_view uniform, GLint unit) const noexcept;

  void bind() const noexcept;
  void unbind() const noexcept;

  static Expected<Texture2D> getPlaceholder() noexcept;

private:
  explicit Texture2D(
      unsigned int pid, int pwidth, int pheight, int channels) noexcept;

  unsigned int id;
  mutable GLint unit;
  int width;
  int height;
  int channels;
};
} // namespace redoom::graphics
