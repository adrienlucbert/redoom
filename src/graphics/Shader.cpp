#include <redoom/graphics/Shader.hh>

#include <fstream>
#include <sstream>
#include <stdexcept>

#include <fmt/format.h>

namespace redoom::graphics
{
Shader::Shader(unsigned int pid) noexcept
  : id{pid}
{
}

Shader::~Shader() noexcept
{
  glDeleteShader(this->id);
}

Shader Shader::fromString(std::string_view source, unsigned int shader_type)
{
  auto id = glCreateShader(shader_type);
  const auto* const source_data = source.data();
  glShaderSource(id, 1, &source_data, nullptr);
  glCompileShader(id);

  int success;       // NOLINT
  char infoLog[512]; // NOLINT
  glGetShaderiv(id, GL_COMPILE_STATUS, &success);
  if (success == 0) {
    glGetShaderInfoLog(id, 512, nullptr, infoLog);
    throw std::invalid_argument{
        fmt::format("Shader compilation failed: {}", infoLog)};
  }
  return Shader{id};
}

Shader Shader::fromFile(
    std::filesystem::path const& path, unsigned int shader_type)
{
  auto ifs = std::ifstream{path};
  if (!ifs.is_open())
    throw std::invalid_argument{
        fmt::format("Could not open file: {}", path.c_str())};
  auto sstream = std::stringstream{};
  sstream << ifs.rdbuf();
  return Shader::fromString(sstream.str(), shader_type);
}

unsigned int Shader::getId() const noexcept
{
  return this->id;
}
} // namespace redoom::graphics
