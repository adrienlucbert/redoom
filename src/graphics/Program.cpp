#include <redoom/graphics/Program.hh>

#include <stdexcept>

#include <fmt/format.h>

namespace redoom::graphics
{
Program::Program(unsigned int pid) noexcept
  : id{pid}
{
}

Program::~Program() noexcept
{
  glDeleteProgram(this->id);
}

Program Program::create(
    Shader const& vertex_shader, Shader const& fragment_shader)
{
  auto id = glCreateProgram();
  glAttachShader(id, vertex_shader.getId());
  glAttachShader(id, fragment_shader.getId());
  glLinkProgram(id);
  int success;       // NOLINT
  char infoLog[512]; // NOLINT
  glGetProgramiv(id, GL_LINK_STATUS, &success);
  if (success == 0) {
    glGetProgramInfoLog(id, 512, nullptr, infoLog);
    throw std::invalid_argument{
        fmt::format("Program linking failed: {}", infoLog)};
  }
  return Program{id};
}

unsigned int Program::getId() const noexcept
{
  return this->id;
}

void Program::use() const noexcept
{
  glUseProgram(this->id);
}
} // namespace redoom::graphics
