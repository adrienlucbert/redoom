#pragma once

#include <memory>

#include <glm/vec4.hpp>

#include <Utils/Expected.hh>

namespace redoom::renderer
{
struct RendererViewport {
  int x;
  int y;
  int width;
  int height;
};

class RendererAPI
{
public:
  enum class API { Unknown, OpenGL };

  RendererAPI() noexcept = default;
  RendererAPI(RendererAPI const& b) noexcept = default;
  RendererAPI(RendererAPI&& b) noexcept = default;
  virtual ~RendererAPI() noexcept = default;

  RendererAPI& operator=(RendererAPI const& rhs) noexcept = default;
  RendererAPI& operator=(RendererAPI&& rhs) noexcept = default;

  virtual void setViewport(RendererViewport viewport) noexcept = 0;
  virtual void setClearColor(const glm::vec4& color) noexcept = 0;
  virtual void clear() noexcept = 0;

  [[nodiscard]] static API getAPI() noexcept;

  [[nodiscard]] static Expected<std::unique_ptr<renderer::RendererAPI>>
  create() noexcept;
};
}; // namespace redoom::renderer
