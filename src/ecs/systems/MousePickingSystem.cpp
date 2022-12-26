#include <redoom/ecs/systems/MousePickingSystem.hh>

#include <GLFW/glfw3.h>
#include <stb/stb_image_write.h>

#include <Utils/Concepts.hpp>
#include <redoom/Runtime.hh>
#include <redoom/ecs/components/MaterialComponent.hh>
#include <redoom/ecs/components/MeshComponent.hh>
#include <redoom/ecs/components/ModelComponent.hh>
#include <redoom/ecs/components/TransformComponent.hh>
#include <redoom/graphics/ShaderLibrary.hh>
#include <redoom/renderer/Renderer.hh>

namespace redoom::ecs::systems
{
namespace detail
{
template <concepts::Drawable T>
static void renderDrawable(
    UpdateContext& context, Entity entity, T const& drawable)
{
  static auto program_opt = graphics::ShaderLibrary::getShader("mouse_picking");
  if (!program_opt) {
    std::cerr << "Undefined shader" << '\n';
    return;
  }
  static auto& program = *program_opt; // NOLINT

  auto model = glm::mat4(1.0f);
  auto transform_opt =
      context.getComponentManager().get<components::TransformComponent>(entity);
  if (transform_opt) {
    auto& transform = *transform_opt;
    model = glm::translate(model, transform.getPosition());
    model = glm::scale(model, transform.getScale());
    model = glm::rotate(model, transform.getAngle(), transform.getRotation());
  }
  renderer::Renderer::get().setGlobalUniform("EntityId",
      graphics::uniforms::Uint{.value = static_cast<GLuint>(entity)});
  renderer::Renderer::get().useProgram(program);
  renderer::Renderer::get().draw(drawable, model);
}
} // namespace detail

void MousePickingSystem::update(UpdateContext& context) noexcept
{
  static auto framebuffer =
      graphics::FrameBuffer{redoom::Runtime::get().getWindow().getWidth(),
          redoom::Runtime::get().getWindow().getHeight()};
  framebuffer.bind();
  glEnable(GL_DEPTH_TEST);
  glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

  context.getComponentManager().apply<components::MeshComponent>(
      [&](auto entity, components::MeshComponent& component) {
        detail::renderDrawable(context, entity, *component.mesh_);
      });
  context.getComponentManager().apply<components::ModelComponent>(
      [&](auto entity, components::ModelComponent& component) {
        detail::renderDrawable(context, entity, component.model_);
      });

  double xpos = 0;
  double ypos = 0;
  glfwGetCursorPos(static_cast<GLFWwindow*>(
                       redoom::Runtime::get().getWindow().getNativeWindow()),
      &xpos,
      &ypos);
  if (xpos >= 0.0 && xpos <= redoom::Runtime::get().getWindow().getWidth()
      && ypos >= 0.0
      && ypos <= redoom::Runtime::get().getWindow().getHeight()) {
    glFlush();
    glFinish();

    glPixelStorei(GL_UNPACK_ALIGNMENT, 1);

    float pixel[3]; // NOLINT
    glReadPixels(static_cast<int>(xpos),
        redoom::Runtime::get().getWindow().getHeight() - static_cast<int>(ypos),
        1,
        1,
        GL_RGB,
        GL_FLOAT,
        &pixel);

    auto entity_id = ((static_cast<unsigned int>(pixel[0] * 255.0f) << 0u)
                      + (static_cast<unsigned int>(pixel[1] * 255.0f) << 8u)
                      + (static_cast<unsigned int>(pixel[2] * 255.0f) << 16u));

    if (entity_id != (255 << 0) + (255 << 8) + (255 << 16)) {
      std::cout << fmt::format("({} {}): [{}/{}]",
          static_cast<int>(xpos),
          static_cast<int>(ypos),
          entity_id,
          (255 << 0) + (255 << 8) + (255 << 16))
                << '\n';
    }
  }
  framebuffer.unbind();
}
} // namespace redoom::ecs::systems
