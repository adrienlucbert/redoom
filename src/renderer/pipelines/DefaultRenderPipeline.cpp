#include <redoom/renderer/pipelines/DefaultRenderPipeline.hh>

#include <redoom/Application.hh>
#include <redoom/graphics/FrameBufferLibrary.hh>
#include <redoom/graphics/ShaderLibrary.hh>
#include <redoom/graphics/mesh/Quad.hh>

namespace redoom::renderer::pipelines
{
DefaultRenderPipeline::DefaultRenderPipeline() noexcept
  : framebuffer_{graphics::FrameBufferLibrary::addFrameBuffer(
      "default", 1000, 800)}
  , screen_quad_{graphics::mesh::Quad{2.0f, 2.0f}}
  , shader_{*graphics::ShaderLibrary::getShader("draw_framebuffer")}
{
}

graphics::Material const& DefaultRenderPipeline::getDefaultMaterial()
    const noexcept
{
  static auto const default_material = graphics::Material{
      "lit", 6.0f, 1.0f, 1.0f, glm::vec3{1.0f, 1.0f, 1.0f}, {}, GL_TRIANGLES};
  return default_material;
}

graphics::FrameBuffer const& DefaultRenderPipeline::getFrameBuffer()
    const noexcept
{
  return this->framebuffer_;
}

graphics::FrameBuffer& DefaultRenderPipeline::getFrameBuffer() noexcept
{
  return this->framebuffer_;
}

void DefaultRenderPipeline::beginRendering() noexcept
{
  this->framebuffer_.get().bind();
  glEnable(GL_DEPTH_TEST);
  glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

void DefaultRenderPipeline::endRendering() noexcept
{
  this->framebuffer_.get().unbind();
  auto is_wireframe = Renderer::get().getAPI().isWireframe();
  Renderer::get().getAPI().setWireframe(false);
  glDisable(GL_DEPTH_TEST);
  glClearColor(0.5f, 0.0f, 0.5f, 1.0f);
  glClear(GL_COLOR_BUFFER_BIT);
  auto const& framebuffer_texture = this->framebuffer_.get().getTexture();
  this->shader_.get().use();
  framebuffer_texture.setUnit("framebuffer", 0u);
  framebuffer_texture.bind();
  this->screen_quad_.draw();
  framebuffer_texture.unbind();
  Renderer::get().getAPI().setWireframe(is_wireframe);
}
} // namespace redoom::renderer::pipelines
