#pragma once

#include <memory>

#include <Utils/Expected.hh>

namespace redoom::renderer
{
class RendererContext
{
public:
  RendererContext(RendererContext const&) noexcept = delete;
  RendererContext(RendererContext&&) noexcept = default;
  virtual ~RendererContext() noexcept = default;

  RendererContext& operator=(RendererContext const&) noexcept = delete;
  RendererContext& operator=(RendererContext&&) noexcept = default;

  virtual void swapBuffers() noexcept = 0;

  static Expected<std::unique_ptr<RendererContext>> create(
      void* native_window) noexcept;

protected:
  RendererContext() noexcept = default;
};
} // namespace redoom::renderer
