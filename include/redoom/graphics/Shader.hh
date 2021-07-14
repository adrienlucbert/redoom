#pragma once

#include <filesystem>
#include <string_view>

#include <GL/glew.h>

namespace redoom::graphics
{
class Shader
{
public:
  Shader(Shader const& b) noexcept = delete;
  Shader(Shader&& b) noexcept = default;
  ~Shader() noexcept;

  Shader& operator=(Shader const& rhs) noexcept = delete;
  Shader& operator=(Shader&& rhs) noexcept = default;

  [[nodiscard]] static Shader fromString(
      std::string_view source, unsigned int shader_type);
  [[nodiscard]] static Shader fromFile(
      std::filesystem::path const& path, unsigned int shader_type);

  [[nodiscard]] unsigned int getId() const noexcept;

private:
  explicit Shader(unsigned int pid) noexcept;

  unsigned int id;
};

struct VertexShader : public Shader {
  [[nodiscard]] static Shader fromString(std::string_view source)
  {
    return Shader::fromString(source, GL_VERTEX_SHADER);
  }
  [[nodiscard]] static Shader fromFile(std::filesystem::path const& path)
  {
    return Shader::fromFile(path, GL_VERTEX_SHADER);
  }
};

struct FragmentShader : public Shader {
  [[nodiscard]] static Shader fromString(std::string_view source)
  {
    return Shader::fromString(source, GL_FRAGMENT_SHADER);
  }
  [[nodiscard]] static Shader fromFile(std::filesystem::path const& path)
  {
    return Shader::fromFile(path, GL_FRAGMENT_SHADER);
  }
};
} // namespace redoom::graphics
