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

Shader::Shader(Shader&& b) noexcept
  : id{b.id}
{
  b.id = 0;
}

Shader::~Shader() noexcept
{
  if (this->id != 0)
    glDeleteShader(this->id);
}

Shader& Shader::operator=(Shader&& rhs) noexcept
{
  if (this != &rhs)
    std::swap(this->id, rhs.id);
  return *this;
}

Expected<Shader> Shader::fromString(
    std::string_view source, unsigned int shader_type)
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
    return make_formatted_unexpected("Shader compilation failed: {}", infoLog);
  }
  return Shader{id};
}

Expected<Shader> Shader::fromFile(
    std::filesystem::path const& path, unsigned int shader_type)
{
  auto ifs = std::ifstream{path};
  if (!ifs.is_open())
    return make_formatted_unexpected("Could not open file: {}", path.c_str());
  auto sstream = std::stringstream{};
  sstream << ifs.rdbuf();
  return Shader::fromString(sstream.str(), shader_type);
}

unsigned int Shader::getId() const noexcept
{
  return this->id;
}
} // namespace redoom::graphics
