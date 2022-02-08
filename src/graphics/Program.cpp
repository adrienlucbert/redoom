#include <redoom/graphics/Program.hh>

#include <stdexcept>

namespace redoom::graphics
{
Program::Program(unsigned int pid) noexcept
  : id{pid}
{
}

Program::Program(Program&& b) noexcept
  : id{b.id}
{
  b.id = 0;
}

Program::~Program() noexcept
{
  if (this->id != 0)
    glDeleteProgram(this->id);
}

Program& Program::operator=(Program&& rhs) noexcept
{
  if (this != &rhs)
    std::swap(this->id, rhs.id);
  return *this;
}

Expected<Program> Program::create(
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
    return make_formatted_unexpected("Program linking failed: {}", infoLog);
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
