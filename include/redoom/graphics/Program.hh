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

  void use() const noexcept;

private:
  explicit Program(unsigned int pid) noexcept;

  unsigned int id;
};
} // namespace redoom::graphics
