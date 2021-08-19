#include <mock/Window.hh>

namespace mock
{
int TestWindow::getWidth() const noexcept
{
  return 0;
}
int TestWindow::getHeight() const noexcept
{
  return 0;
}

void TestWindow::setVSync(bool /*enable*/) noexcept
{
}
bool TestWindow::hasVSync() noexcept
{
  return false;
}

void* TestWindow::getNativeWindow() const noexcept
{
  return nullptr;
}

void TestWindow::setCursorMode(TestWindow::CursorMode /*mode*/) noexcept
{
}

bool TestWindow::pollEvent(redoom::events::Event& /*buffer*/) noexcept
{
  return false;
}

void TestWindow::onUpdate() noexcept
{
}
} // namespace mock
