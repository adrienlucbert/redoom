#include <redoom/graphics/Program.hh>

#include <redoom/graphics/BoundUniform.hh>

namespace redoom::graphics
{
Program::Program(unsigned int id) noexcept
  : id_{id}
{
}

Program::Program(Program&& b) noexcept
  : id_{b.id_}
{
  b.id_ = 0;
}

Program::~Program() noexcept
{
  if (this->id_ != 0)
    glDeleteProgram(this->id_);
}

Program& Program::operator=(Program&& rhs) noexcept
{
  if (this != &rhs)
    std::swap(this->id_, rhs.id_);
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
  return this->id_;
}

void Program::use() const noexcept
{
  glUseProgram(this->id_);
}

void Program::apply() const noexcept
{
  for (auto const& uniform : this->uniforms_)
    uniform.second->set(*this, uniform.first);
}
} // namespace redoom::graphics
