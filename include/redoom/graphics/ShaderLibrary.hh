#pragma once

#include <tl/optional.hpp>

#include <Utils/Expected.hh>
#include <redoom/graphics/Program.hh>

namespace redoom::graphics
{
class ShaderLibrary
{
public:
  static Expected<std::reference_wrapper<Program>> addShader(std::string name,
      std::filesystem::path const& vertex_shader,
      std::filesystem::path const& fragment_shader) noexcept;
  static Program& addShader(std::string name, Program program) noexcept;
  static tl::optional<Program&> getShader(std::string_view name) noexcept;

private:
  static std::unordered_map<std::string, Program> shaders_; // NOLINT
};
} // namespace redoom::graphics
