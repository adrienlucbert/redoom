#pragma once

#include <redoom/renderer/Window.hh>

namespace mock
{
class TestWindow : public redoom::renderer::Window
{
public:
  TestWindow() noexcept = default;
  TestWindow(TestWindow const& b) noexcept = delete;
  TestWindow(TestWindow&& b) noexcept = default;
  ~TestWindow() noexcept override = default;

  TestWindow& operator=(TestWindow const& rhs) noexcept = delete;
  TestWindow& operator=(TestWindow&& rhs) noexcept = default;

  [[nodiscard]] int getWidth() const noexcept override;
  [[nodiscard]] int getHeight() const noexcept override;

  void setVSync(bool enable) noexcept override;
  [[nodiscard]] bool hasVSync() noexcept override;

  [[nodiscard]] void* getNativeWindow() const noexcept override;

  void setCursorMode(CursorMode /*mode*/) noexcept override;

  [[nodiscard]] bool pollEvent(redoom::events::Event& buffer) noexcept override;

  void onUpdate() noexcept override;
};
} // namespace mock
