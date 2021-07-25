#pragma once

#include <filesystem>

#include <GL/glew.h>

#include <Utils/Expected.hh>

namespace redoom::graphics
{
class Texture2D
{
public:
  Texture2D(Texture2D const& b) noexcept = default;
  Texture2D(Texture2D&& b) noexcept;
  ~Texture2D() noexcept;

  Texture2D& operator=(Texture2D const& rhs) noexcept = default;
  Texture2D& operator=(Texture2D&& rhs) noexcept;

  [[nodiscard]] static Expected<Texture2D> fromFile(
      std::filesystem::path const& path) noexcept;

  [[nodiscard]] unsigned int getId() const noexcept;
  [[nodiscard]] int getWidth() const noexcept;
  [[nodiscard]] int getHeight() const noexcept;
  [[nodiscard]] int getChannels() const noexcept;

  void setUnit(GLenum texture_unit) const noexcept;

  void bind() const noexcept;
  void unbind() const noexcept;

private:
  explicit Texture2D(
      unsigned int pid, int pwidth, int pheight, int pchannels) noexcept;

  unsigned int id;
  int width;
  int height;
  int channels;
};
} // namespace redoom::graphics
