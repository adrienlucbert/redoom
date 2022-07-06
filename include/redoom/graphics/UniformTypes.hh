#pragma once

#include <GL/glew.h>
#include <glm/gtc/type_ptr.hpp>

#include <redoom/graphics/UniformTypeSetter.hh>

namespace redoom::graphics
{
namespace uniforms
{
namespace detail
{
struct UniformType {
};
} // namespace detail

struct Bool : public detail::UniformType {
  GLboolean value;
};

struct Int : public detail::UniformType {
  GLint value;
};

struct Uint : public detail::UniformType {
  GLuint value;
};

struct Float : public detail::UniformType {
  GLfloat value;
};

template <unsigned long Size, typename T = GLfloat>
struct Vector : public detail::UniformType {
  glm::vec<Size, T> const& value;
};

template <unsigned long X, unsigned long Y, typename T = GLfloat>
struct Matrix : public detail::UniformType {
  glm::mat<X, Y, T> const& value;
  GLboolean transpose;
};
} // namespace uniforms

template <>
struct UniformTypeSetter<uniforms::Bool> {
  static void set(int location, std::any const& data) noexcept
  {
    auto const& uniform_data = std::any_cast<uniforms::Bool>(data);
    glUniform1i(location, static_cast<GLint>(uniform_data.value));
  }
};

template <>
struct UniformTypeSetter<uniforms::Int> {
  static void set(int location, std::any const& data) noexcept
  {
    auto const& uniform_data = std::any_cast<uniforms::Int>(data);
    glUniform1i(location, static_cast<GLint>(uniform_data.value));
  }
};

template <>
struct UniformTypeSetter<uniforms::Uint> {
  static void set(int location, std::any const& data) noexcept
  {
    auto const& uniform_data = std::any_cast<uniforms::Uint>(data);
    glUniform1ui(location, static_cast<GLuint>(uniform_data.value));
  }
};

template <>
struct UniformTypeSetter<uniforms::Float> {
  static void set(int location, std::any const& data) noexcept
  {
    auto const& uniform_data = std::any_cast<uniforms::Float>(data);
    glUniform1f(location, static_cast<GLfloat>(uniform_data.value));
  }
};

template <unsigned long Size>
struct UniformTypeSetter<uniforms::Vector<Size, GLboolean>> {
  static void set(int location, std::any const& data) noexcept
  {
    auto const& uniform_data =
        std::any_cast<uniforms::Vector<Size, GLboolean>>(data);
    if constexpr (Size == 1)
      glUniform1i(location, static_cast<GLint>(uniform_data.value[0]));
    else if constexpr (Size == 2)
      glUniform2i(location,
          static_cast<GLint>(uniform_data.value[0]),
          static_cast<GLint>(uniform_data.value[1]));
    else if constexpr (Size == 3)
      glUniform3i(location,
          static_cast<GLint>(uniform_data.value[0]),
          static_cast<GLint>(uniform_data.value[1]),
          static_cast<GLint>(uniform_data.value[2]));
    else if constexpr (Size == 4)
      glUniform4i(location,
          static_cast<GLint>(uniform_data.value[0]),
          static_cast<GLint>(uniform_data.value[1]),
          static_cast<GLint>(uniform_data.value[2]),
          static_cast<GLint>(uniform_data.value[3]));
    else
      static_assert("Unsupported uniform vector size" != nullptr);
  }
};

template <unsigned long Size>
struct UniformTypeSetter<uniforms::Vector<Size, GLint>> {
  static void set(int location, std::any const& data) noexcept
  {
    auto const& uniform_data =
        std::any_cast<uniforms::Vector<Size, GLint>>(data);
    if constexpr (Size == 1)
      glUniform1iv(location, 1, glm::value_ptr(uniform_data.value));
    else if constexpr (Size == 2)
      glUniform2iv(location, 1, glm::value_ptr(uniform_data.value));
    else if constexpr (Size == 3)
      glUniform3iv(location, 1, glm::value_ptr(uniform_data.value));
    else if constexpr (Size == 4)
      glUniform4iv(location, 1, glm::value_ptr(uniform_data.value));
    else
      static_assert("Unsupported uniform vector size" != nullptr);
  }
};

template <unsigned long Size>
struct UniformTypeSetter<uniforms::Vector<Size, GLuint>> {
  static void set(int location, std::any const& data) noexcept
  {
    auto const& uniform_data =
        std::any_cast<uniforms::Vector<Size, GLuint>>(data);
    if constexpr (Size == 1)
      glUniform1uiv(location, 1, glm::value_ptr(uniform_data.value));
    else if constexpr (Size == 2)
      glUniform2uiv(location, 1, glm::value_ptr(uniform_data.value));
    else if constexpr (Size == 3)
      glUniform3uiv(location, 1, glm::value_ptr(uniform_data.value));
    else if constexpr (Size == 4)
      glUniform4uiv(location, 1, glm::value_ptr(uniform_data.value));
    else
      static_assert("Unsupported uniform vector size" != nullptr);
  }
};

template <unsigned long Size>
struct UniformTypeSetter<uniforms::Vector<Size, GLfloat>> {
  static void set(int location, std::any const& data) noexcept
  {
    auto const& uniform_data =
        std::any_cast<uniforms::Vector<Size, GLfloat>>(data);
    if constexpr (Size == 1)
      glUniform1fv(location, 1, glm::value_ptr(uniform_data.value));
    else if constexpr (Size == 2)
      glUniform2fv(location, 1, glm::value_ptr(uniform_data.value));
    else if constexpr (Size == 3)
      glUniform3fv(location, 1, glm::value_ptr(uniform_data.value));
    else if constexpr (Size == 4)
      glUniform4fv(location, 1, glm::value_ptr(uniform_data.value));
    else
      static_assert("Unsupported uniform vector size" != nullptr);
  }
};

template <>
struct UniformTypeSetter<uniforms::Matrix<2, 2>> {
  static void set(int location, std::any const& data) noexcept
  {
    auto const& uniform_data = std::any_cast<uniforms::Matrix<2, 2>>(data);
    glUniformMatrix2fv(location,
        1,
        uniform_data.transpose,
        glm::value_ptr(uniform_data.value));
  }
};

template <>
struct UniformTypeSetter<uniforms::Matrix<3, 3>> {
  static void set(int location, std::any const& data) noexcept
  {
    auto const& uniform_data = std::any_cast<uniforms::Matrix<3, 3>>(data);
    glUniformMatrix3fv(location,
        1,
        uniform_data.transpose,
        glm::value_ptr(uniform_data.value));
  }
};

template <>
struct UniformTypeSetter<uniforms::Matrix<4, 4>> {
  static void set(int location, std::any const& data) noexcept
  {
    auto const& uniform_data = std::any_cast<uniforms::Matrix<4, 4>>(data);
    glUniformMatrix4fv(location,
        1,
        uniform_data.transpose,
        glm::value_ptr(uniform_data.value));
  }
};

template <>
struct UniformTypeSetter<uniforms::Matrix<2, 3>> {
  static void set(int location, std::any const& data) noexcept
  {
    auto const& uniform_data = std::any_cast<uniforms::Matrix<2, 3>>(data);
    glUniformMatrix2x3fv(location,
        1,
        uniform_data.transpose,
        glm::value_ptr(uniform_data.value));
  }
};

template <>
struct UniformTypeSetter<uniforms::Matrix<3, 2>> {
  static void set(int location, std::any const& data) noexcept
  {
    auto const& uniform_data = std::any_cast<uniforms::Matrix<3, 2>>(data);
    glUniformMatrix3x2fv(location,
        1,
        uniform_data.transpose,
        glm::value_ptr(uniform_data.value));
  }
};

template <>
struct UniformTypeSetter<uniforms::Matrix<2, 4>> {
  static void set(int location, std::any const& data) noexcept
  {
    auto const& uniform_data = std::any_cast<uniforms::Matrix<2, 4>>(data);
    glUniformMatrix2x4fv(location,
        1,
        uniform_data.transpose,
        glm::value_ptr(uniform_data.value));
  }
};

template <>
struct UniformTypeSetter<uniforms::Matrix<4, 2>> {
  static void set(int location, std::any const& data) noexcept
  {
    auto const& uniform_data = std::any_cast<uniforms::Matrix<4, 2>>(data);
    glUniformMatrix4x2fv(location,
        1,
        uniform_data.transpose,
        glm::value_ptr(uniform_data.value));
  }
};

template <>
struct UniformTypeSetter<uniforms::Matrix<3, 4>> {
  static void set(int location, std::any const& data) noexcept
  {
    auto const& uniform_data = std::any_cast<uniforms::Matrix<3, 4>>(data);
    glUniformMatrix3x4fv(location,
        1,
        uniform_data.transpose,
        glm::value_ptr(uniform_data.value));
  }
};

template <>
struct UniformTypeSetter<uniforms::Matrix<4, 3>> {
  static void set(int location, std::any const& data) noexcept
  {
    auto const& uniform_data = std::any_cast<uniforms::Matrix<4, 3>>(data);
    glUniformMatrix4x3fv(location,
        1,
        uniform_data.transpose,
        glm::value_ptr(uniform_data.value));
  }
};
} // namespace redoom::graphics
