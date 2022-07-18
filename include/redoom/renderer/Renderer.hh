#pragma once

#include <glm/gtc/type_ptr.hpp>
#include <tl/optional.hpp>

#include <Utils/Concepts.hpp>
#include <Utils/Singleton.hh>
#include <redoom/graphics/FrameBuffer.hh>
#include <redoom/graphics/Program.hh>
#include <redoom/graphics/UnboundUniform.hh>
#include <redoom/graphics/UniformBase.hh>
#include <redoom/graphics/UniformTypes.hh>
#include <redoom/renderer/RenderPipeline.hh>
#include <redoom/renderer/RendererAPI.hh>
#include <redoom/renderer/Window.hh>

namespace redoom::renderer
{
class Renderer : public Utils::Singleton<Renderer>
{
public:
  RendererAPI& getAPI() noexcept;

  template <typename UniformT>
  void setUniform(std::string_view key, UniformT uniform) noexcept
  {
    this->current_program_.value().setUniform(key, std::move(uniform));
  }

  template <typename UniformT>
  void setGlobalUniform(std::string_view key, UniformT uniform) noexcept
  {
    static_assert(
        std::is_base_of_v<graphics::uniforms::detail::UniformType, UniformT>);
    auto ptr = std::unique_ptr<graphics::UnboundUniform<UniformT>>(
        new graphics::UnboundUniform<UniformT>(std::move(uniform))); // NOLINT
    auto it = this->global_uniforms_.find((key.data()));
    if (it == this->global_uniforms_.end())
      this->global_uniforms_.emplace(key, std::move(ptr));
    else
      it->second = std::move(ptr);
  }

  void useProgram(graphics::Program& program) noexcept;

  void setTopology(GLenum topology) noexcept;
  [[nodiscard]] GLenum getTopology() const noexcept;

  [[nodiscard]] RenderPipeline const& getRenderPipeline() const noexcept;

  void beginRendering() noexcept;
  void endRendering() noexcept;

  template <concepts::Drawable T>
  void draw(T const& drawable, const glm::mat4& model) noexcept
  {
    this->prepareDraw(model);
    drawable.draw();
  }

  void useTexture(graphics::Texture2D const& texture) const noexcept;
  void useTextures(
      std::vector<std::reference_wrapper<graphics::Texture2D>> const& textures)
      const noexcept;

protected:
  explicit Renderer(std::unique_ptr<RendererAPI> api,
      std::unique_ptr<RenderPipeline> rp) noexcept;

  void prepareDraw(const glm::mat4& model) noexcept;

  GLenum topology_{GL_TRIANGLES};
  tl::optional<graphics::Program&> current_program_{tl::nullopt};
  std::unique_ptr<RendererAPI> api_;
  std::unordered_map<std::string, std::unique_ptr<graphics::UniformBase>>
      global_uniforms_{};
  std::unique_ptr<RenderPipeline> rp_;
};
} // namespace redoom::renderer
