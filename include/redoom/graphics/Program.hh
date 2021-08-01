#pragma once

#include <cassert>
#include <initializer_list>
#include <string_view>

#include <Utils/Expected.hh>
#include <Utils/traits.hpp>
#include <redoom/graphics/Shader.hh>

namespace redoom::graphics
{
class Program
{
public:
  Program(Program const& b) noexcept = delete;
  Program(Program&& b) noexcept;
  ~Program() noexcept;

  Program& operator=(Program const& rhs) noexcept = delete;
  Program& operator=(Program&& rhs) noexcept;

  [[nodiscard]] static Expected<Program> create(
      Shader const& vertex_shader, Shader const& fragment_shader);

  [[nodiscard]] unsigned int getId() const noexcept;

  template <typename... Ts,
      typename = Utils::are_same<bool, std::remove_cvref_t<Ts>...>>
  void setUniform(std::string_view name, bool arg1, Ts&&... args) const noexcept
  {
    if constexpr (sizeof...(Ts) == 0)
      glUniform1i(glGetUniformLocation(this->id, name.data()),
          static_cast<GLint>(arg1));
    else if constexpr (sizeof...(Ts) == 1)
      glUniform2i(glGetUniformLocation(this->id, name.data()),
          static_cast<GLint>(arg1),
          static_cast<GLint>(std::forward<Ts>(args))...);
    else if constexpr (sizeof...(Ts) == 2)
      glUniform3i(glGetUniformLocation(this->id, name.data()),
          static_cast<GLint>(arg1),
          static_cast<GLint>(std::forward<Ts>(args))...);
    else if constexpr (sizeof...(Ts) == 3)
      glUniform4i(glGetUniformLocation(this->id, name.data()),
          static_cast<GLint>(arg1),
          static_cast<GLint>(std::forward<Ts>(args))...);
  }
  template <typename... Ts,
      typename = Utils::are_same<GLint, std::remove_cvref_t<Ts>...>>
  void setUniform(
      std::string_view name, GLint arg1, Ts&&... args) const noexcept
  {
    if constexpr (sizeof...(Ts) == 0)
      glUniform1i(glGetUniformLocation(this->id, name.data()), arg1);
    else if constexpr (sizeof...(Ts) == 1)
      glUniform2i(glGetUniformLocation(this->id, name.data()),
          arg1,
          std::forward<Ts>(args)...);
    else if constexpr (sizeof...(Ts) == 2)
      glUniform3i(glGetUniformLocation(this->id, name.data()),
          arg1,
          std::forward<Ts>(args)...);
    else if constexpr (sizeof...(Ts) == 3)
      glUniform4i(glGetUniformLocation(this->id, name.data()),
          arg1,
          std::forward<Ts>(args)...);
  }
  template <typename... Ts,
      typename = Utils::are_same<GLfloat, std::remove_cvref_t<Ts>...>>
  void setUniform(
      std::string_view name, GLfloat arg1, Ts&&... args) const noexcept
  {
    if constexpr (sizeof...(Ts) == 0)
      glUniform1f(glGetUniformLocation(this->id, name.data()), arg1);
    else if constexpr (sizeof...(Ts) == 1)
      glUniform2f(glGetUniformLocation(this->id, name.data()),
          arg1,
          std::forward<Ts>(args)...);
    else if constexpr (sizeof...(Ts) == 2)
      glUniform3f(glGetUniformLocation(this->id, name.data()),
          arg1,
          std::forward<Ts>(args)...);
    else if constexpr (sizeof...(Ts) == 3)
      glUniform4f(glGetUniformLocation(this->id, name.data()),
          arg1,
          std::forward<Ts>(args)...);
  }
  template <typename... Ts,
      typename = Utils::are_same<GLuint, std::remove_cvref_t<Ts>...>>
  void setUniform(
      std::string_view name, GLuint arg1, Ts&&... args) const noexcept
  {
    if constexpr (sizeof...(Ts) == 0)
      glUniform1ui(glGetUniformLocation(this->id, name.data()), arg1);
    else if constexpr (sizeof...(Ts) == 1)
      glUniform2ui(glGetUniformLocation(this->id, name.data()),
          arg1,
          std::forward<Ts>(args)...);
    else if constexpr (sizeof...(Ts) == 2)
      glUniform3ui(glGetUniformLocation(this->id, name.data()),
          arg1,
          std::forward<Ts>(args)...);
    else if constexpr (sizeof...(Ts) == 3)
      glUniform4ui(glGetUniformLocation(this->id, name.data()),
          arg1,
          std::forward<Ts>(args)...);
  }
  template <typename T>
  void setUniform1v(
      std::string_view name, GLsizei count, T const* value) const noexcept
  {
    if constexpr (std::is_same_v<GLfloat, T>)
      glUniform1fv(glGetUniformLocation(this->id, name.data()), count, value);
    else if constexpr (std::is_same_v<GLint, T>)
      glUniform1iv(glGetUniformLocation(this->id, name.data()), count, value);
    else if constexpr (std::is_same_v<GLuint, T>)
      glUniform1uiv(glGetUniformLocation(this->id, name.data()), count, value);
    else
      static_assert("Unsupported type T" != nullptr);
  }
  template <typename T>
  void setUniform2v(
      std::string_view name, GLsizei count, T const* value) const noexcept
  {
    if constexpr (std::is_same_v<GLfloat, T>)
      glUniform2fv(glGetUniformLocation(this->id, name.data()), count, value);
    else if constexpr (std::is_same_v<GLint, T>)
      glUniform2iv(glGetUniformLocation(this->id, name.data()), count, value);
    else if constexpr (std::is_same_v<GLuint, T>)
      glUniform2uiv(glGetUniformLocation(this->id, name.data()), count, value);
    else
      static_assert("Unsupported type T" != nullptr);
  }
  template <typename T>
  void setUniform3v(
      std::string_view name, GLsizei count, T const* value) const noexcept
  {
    if constexpr (std::is_same_v<GLfloat, T>)
      glUniform3fv(glGetUniformLocation(this->id, name.data()), count, value);
    else if constexpr (std::is_same_v<GLint, T>)
      glUniform3iv(glGetUniformLocation(this->id, name.data()), count, value);
    else if constexpr (std::is_same_v<GLuint, T>)
      glUniform3uiv(glGetUniformLocation(this->id, name.data()), count, value);
    else
      static_assert("Unsupported type T" != nullptr);
  }
  template <typename T>
  void setUniform4v(
      std::string_view name, GLsizei count, T const* value) const noexcept
  {
    if constexpr (std::is_same_v<GLfloat, T>)
      glUniform4fv(glGetUniformLocation(this->id, name.data()), count, value);
    else if constexpr (std::is_same_v<GLint, T>)
      glUniform4iv(glGetUniformLocation(this->id, name.data()), count, value);
    else if constexpr (std::is_same_v<GLuint, T>)
      glUniform4uiv(glGetUniformLocation(this->id, name.data()), count, value);
    else
      static_assert("Unsupported type T" != nullptr);
  }
  void setUniformMatrix2(std::string_view name,
      GLsizei count,
      GLboolean transpose,
      float const* value) const noexcept
  {
    glUniformMatrix2fv(
        glGetUniformLocation(this->id, name.data()), count, transpose, value);
  }
  void setUniformMatrix3(std::string_view name,
      GLsizei count,
      GLboolean transpose,
      float const* value) const noexcept
  {
    glUniformMatrix3fv(
        glGetUniformLocation(this->id, name.data()), count, transpose, value);
  }
  void setUniformMatrix4(std::string_view name,
      GLsizei count,
      GLboolean transpose,
      float const* value) const noexcept
  {
    glUniformMatrix4fv(
        glGetUniformLocation(this->id, name.data()), count, transpose, value);
  }
  void setUniformMatrix2x3(std::string_view name,
      GLsizei count,
      GLboolean transpose,
      float const* value) const noexcept
  {
    glUniformMatrix2x3fv(
        glGetUniformLocation(this->id, name.data()), count, transpose, value);
  }
  void setUniformMatrix3x2(std::string_view name,
      GLsizei count,
      GLboolean transpose,
      float const* value) const noexcept
  {
    glUniformMatrix3x2fv(
        glGetUniformLocation(this->id, name.data()), count, transpose, value);
  }
  void setUniformMatrix2x4(std::string_view name,
      GLsizei count,
      GLboolean transpose,
      float const* value) const noexcept
  {
    glUniformMatrix2x4fv(
        glGetUniformLocation(this->id, name.data()), count, transpose, value);
  }
  void setUniformMatrix4x2(std::string_view name,
      GLsizei count,
      GLboolean transpose,
      float const* value) const noexcept
  {
    glUniformMatrix4x2fv(
        glGetUniformLocation(this->id, name.data()), count, transpose, value);
  }
  void setUniformMatrix3x4(std::string_view name,
      GLsizei count,
      GLboolean transpose,
      float const* value) const noexcept
  {
    glUniformMatrix3x4fv(
        glGetUniformLocation(this->id, name.data()), count, transpose, value);
  }
  void setUniformMatrix4x3(std::string_view name,
      GLsizei count,
      GLboolean transpose,
      float const* value) const noexcept
  {
    glUniformMatrix4x3fv(
        glGetUniformLocation(this->id, name.data()), count, transpose, value);
  }

  void use() const noexcept;

private:
  explicit Program(unsigned int pid) noexcept;

  unsigned int id;
};
} // namespace redoom::graphics
