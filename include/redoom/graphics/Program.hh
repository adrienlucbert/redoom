#pragma once

#include <cassert>
#include <functional>
#include <initializer_list>
#include <string_view>
#include <unordered_map>

#include <Utils/Expected.hh>
#include <Utils/traits.hpp>
#include <redoom/graphics/BoundUniform.hh>
#include <redoom/graphics/Shader.hh>
#include <redoom/graphics/UniformBase.hh>
#include <redoom/graphics/UniformTypes.hh>

namespace redoom::graphics
{
class UniformBase;

class Program
{
public:
  Program(Program const&) noexcept = delete;
  Program(Program&& b) noexcept;
  ~Program() noexcept;

  Program& operator=(Program const&) noexcept = delete;
  Program& operator=(Program&& rhs) noexcept;

  [[nodiscard]] static Expected<Program> create(
      Shader const& vertex_shader, Shader const& fragment_shader);

  [[nodiscard]] unsigned int getId() const noexcept;

  template <typename UniformT>
  void setUniform(std::string_view key, UniformT uniform) noexcept
  {
    static_assert(std::is_base_of_v<uniforms::detail::UniformType, UniformT>);
    auto ptr =
        std::unique_ptr<BoundUniform<UniformT>>(new BoundUniform<UniformT>(
            glGetUniformLocation(this->getId(), key.data()),
            std::move(uniform))); // NOLINT
    auto it = this->uniforms_.find((key.data()));
    if (it == this->uniforms_.end())
      this->uniforms_.emplace(key, std::move(ptr));
    else
      it->second = std::move(ptr);
  }

  void use() const noexcept;
  void apply() const noexcept;

private:
  explicit Program(unsigned int id) noexcept;

  unsigned int id_;
  std::unordered_map<std::string, std::unique_ptr<UniformBase>> uniforms_;
};
} // namespace redoom::graphics
