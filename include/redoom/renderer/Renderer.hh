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
  static void draw(
      graphics::Program& program, T& drawable, const glm::mat4& model) noexcept
  {
    program.use();
    program.setUniformMatrix4("projection",
        1,
        GL_FALSE,
        glm::value_ptr(Renderer::getProjectionMatrix()));
    program.setUniformMatrix4(
        "view", 1, GL_FALSE, glm::value_ptr(Renderer::getViewMatrix()));
    program.setUniformMatrix4("model", 1, GL_FALSE, glm::value_ptr(model));
    drawable.draw(program);
  }

private:
  static std::unique_ptr<RendererAPI> api;   // NOLINT
  static glm::mat4 camera_view_matrix;       // NOLINT
  static glm::mat4 camera_projection_matrix; // NOLINT
};
} // namespace redoom::renderer
