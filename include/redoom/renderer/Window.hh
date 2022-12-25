#pragma once

#include <memory>
#include <string_view>

#include <Utils/Expected.hh>
#include <redoom/events/Event.hh>

namespace redoom::renderer
{
using EventCallback = std::function<void(events::Event const&)>;

class Window
{
public:
  enum class CursorMode { Normal, Hidden, Disabled };

  Window() noexcept = default;
  Window(Window const&) noexcept = delete;
  Window(Window&&) noexcept = default;
  virtual ~Window() noexcept = default;

  Window& operator=(Window const&) noexcept = delete;
  Window& operator=(Window&&) noexcept = default;

  [[nodiscard]] virtual int getWidth() const noexcept = 0;
  [[nodiscard]] virtual int getHeight() const noexcept = 0;

  virtual void setVSync(bool enable) noexcept = 0;
  [[nodiscard]] virtual bool hasVSync() noexcept = 0;

  [[nodiscard]] virtual void* getNativeWindow() const noexcept = 0;

  virtual void setCursorMode(CursorMode mode) noexcept = 0;

  virtual void setEventCallback(EventCallback const& callback) noexcept = 0;

  virtual void onUpdate() noexcept = 0;

  [[nodiscard]] static Expected<std::unique_ptr<Window>> create(
      std::string_view title = "Redoom Engine",
      int width = 1600,
      int height = 900,
      EventCallback const& event_callback = nullptr) noexcept;
};
}; // namespace redoom::renderer
