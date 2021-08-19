#pragma once

#include <glm/gtc/type_ptr.hpp>

#include <redoom/graphics/Mesh.hh>
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

  static void draw(graphics::Program& program,
      graphics::Mesh& mesh,
      const glm::mat4& model) noexcept;

private:
  static std::unique_ptr<RendererAPI> api;   // NOLINT
  static glm::mat4 camera_view_matrix;       // NOLINT
  static glm::mat4 camera_projection_matrix; // NOLINT
};
} // namespace redoom::renderer
