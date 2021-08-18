#include <redoom/graphics/ShaderLibrary.hh>

#include <redoom/graphics/Shader.hh>

namespace redoom::graphics
{
// NOLINTNEXTLINE
auto ShaderLibrary::shaders = std::unordered_map<std::string, Program>{};

void ShaderLibrary::addShader(std::string name, Program program) noexcept
{
  ShaderLibrary::shaders.emplace(std::move(name), std::move(program));
}

Expected<> ShaderLibrary::addShader(std::string name,
    std::filesystem::path const& vertex_shader,
    std::filesystem::path const& fragment_shader) noexcept
{
  // Link the shaders to a program
  auto vs_exp = VertexShader::fromFile(vertex_shader);
  RETURN_IF_UNEXPECTED(vs_exp);
  auto& vs = *vs_exp;
  auto fs_exp = FragmentShader::fromFile(fragment_shader);
  RETURN_IF_UNEXPECTED(fs_exp);
  auto& fs = *fs_exp;
  auto program_exp = Program::create(vs, fs);
  RETURN_IF_UNEXPECTED(program_exp);
  ShaderLibrary::addShader(std::move(name), std::move(*program_exp));
  return {};
}

tl::optional<Program&> ShaderLibrary::getShader(std::string_view name) noexcept
{
  auto const& program_it = ShaderLibrary::shaders.find(name.data());
  if (program_it == ShaderLibrary::shaders.end())
    return tl::nullopt;
  else
    return program_it->second;
}
} // namespace redoom::graphics
