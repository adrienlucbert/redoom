#pragma once

#include <redoom/graphics/FrameBuffer.hh>
#include <redoom/graphics/VertexArray.hh>
#include <redoom/graphics/mesh/Quad.hh>
#include <redoom/renderer/RenderPipeline.hh>

namespace redoom::renderer::pipelines
{
class DefaultRenderPipeline : public RenderPipeline
{
public:
  DefaultRenderPipeline() noexcept;

  void beginRendering() noexcept override;
  void endRendering() noexcept override;

  [[nodiscard]] graphics::Material const& getDefaultMaterial()
      const noexcept override;

private:
  graphics::FrameBuffer framebuffer_;
  graphics::mesh::Quad screen_quad_;
  std::reference_wrapper<graphics::Program> shader_;
};
} // namespace redoom::renderer::pipelines
