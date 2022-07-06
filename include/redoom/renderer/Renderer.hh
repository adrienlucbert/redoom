#pragma once

#include <glm/gtc/type_ptr.hpp>

#include <Utils/Concepts.hpp>
#include <redoom/graphics/Mesh.hh>
#include <redoom/graphics/Model.hh>
#include <redoom/graphics/Program.hh>
#include <redoom/graphics/UnboundUniform.hh>
#include <redoom/graphics/UniformBase.hh>
#include <redoom/graphics/UniformTypes.hh>
#include <redoom/renderer/RendererAPI.hh>

namespace redoom::renderer
{
class Renderer
{
public:
  static RendererAPI& getAPI() noexcept;

  template <typename UniformT>
  static void setGlobalUniform(std::string_view key, UniformT uniform) noexcept
  {
    static_assert(
        std::is_base_of_v<graphics::uniforms::detail::UniformType, UniformT>);
    auto ptr = std::unique_ptr<graphics::UnboundUniform<UniformT>>(
        new graphics::UnboundUniform<UniformT>(std::move(uniform))); // NOLINT
    auto it = Renderer::global_uniforms_.find((key.data()));
    if (it == Renderer::global_uniforms_.end())
      Renderer::global_uniforms_.emplace(key, std::move(ptr));
    else
      it->second = std::move(ptr);
  }

  template <concepts::Drawable T>
  static void draw(graphics::Program& program,
      T const& drawable,
      const glm::mat4& model) noexcept
  {
    Renderer::prepareDraw(program, model);
    drawable.draw(program);
  }

  template <concepts::DrawablePtr T>
  static void draw(graphics::Program& program,
      T const& drawable,
      const glm::mat4& model) noexcept
  {
    Renderer::prepareDraw(program, model);
    drawable->draw(program);
  }

private:
  static void prepareDraw(
      graphics::Program& program, const glm::mat4& model) noexcept;

  static std::unique_ptr<RendererAPI> api_; // NOLINT
  static std::unordered_map<std::string, std::unique_ptr<graphics::UniformBase>>
      global_uniforms_; // NOLINT
};
} // namespace redoom::renderer
