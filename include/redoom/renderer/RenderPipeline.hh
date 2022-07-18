#pragma once

#include <functional>

#include <redoom/graphics/Material.hh>
#include <redoom/graphics/Program.hh>

namespace redoom::renderer
{
class RenderPipeline
{
public:
  RenderPipeline() noexcept = default;
  RenderPipeline(RenderPipeline const&) noexcept = delete;
  RenderPipeline(RenderPipeline&&) noexcept = default;
  virtual ~RenderPipeline() noexcept = default;

  RenderPipeline& operator=(RenderPipeline const&) noexcept = delete;
  RenderPipeline& operator=(RenderPipeline&&) noexcept = default;

  [[nodiscard]] virtual graphics::Material const& getDefaultMaterial()
      const noexcept = 0;

  virtual void beginRendering() noexcept = 0;
  virtual void endRendering() noexcept = 0;
};
} // namespace redoom::renderer
