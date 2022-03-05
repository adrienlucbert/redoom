#pragma once

#include <glm/gtc/type_ptr.hpp>

#include <Utils/Concepts.hpp>
#include <redoom/graphics/Mesh.hh>
#include <redoom/graphics/Model.hh>
#include <redoom/graphics/Program.hh>
#include <redoom/renderer/RendererAPI.hh>

namespace redoom::renderer
{
class Renderer
{
public:
  static RendererAPI& getAPI() noexcept;

  static void setViewMatrix(glm::mat4 view) noexcept;
  static glm::mat4 const& getViewMatrix() noexcept;

  static void setProjectionMatrix(glm::mat4 view) noexcept;
  static glm::mat4 const& getProjectionMatrix() noexcept;

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
      graphics::Program& program, const glm::mat4& model) noexcept
  {
    program.use();
    program.setUniformMatrix4("projection",
        1,
        GL_FALSE,
        glm::value_ptr(Renderer::getProjectionMatrix()));
    program.setUniformMatrix4(
        "view", 1, GL_FALSE, glm::value_ptr(Renderer::getViewMatrix()));
    program.setUniformMatrix4("model", 1, GL_FALSE, glm::value_ptr(model));
  }

  static std::unique_ptr<RendererAPI> api_;   // NOLINT
  static glm::mat4 camera_view_matrix_;       // NOLINT
  static glm::mat4 camera_projection_matrix_; // NOLINT
};
} // namespace redoom::renderer
