#include <redoom/Application.hh>

#include <cassert>
#include <iostream>

#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include <redoom/renderer/Renderer.hh>
#include <redoom/renderer/Window.hh>

namespace redoom
{
bool Application::isReady() const noexcept
{
  return bool(this->window);
}

void Application::run() noexcept
{
  while (glfwWindowShouldClose(
             static_cast<GLFWwindow*>(this->window->getNativeWindow()))
         == GL_FALSE) {
    auto current_time = glfwGetTime();
    auto const elapsed_time = current_time - this->previous_time;
    this->previous_time = current_time;

    renderer::Renderer::getAPI().setClearColor({0.2f, 0.3f, 0.3f, 1.0f});
    renderer::Renderer::getAPI().clear();

    this->registry.update(*this->window, elapsed_time);

    this->window->onUpdate();
  }
}

ecs::Registry& Application::getRegistry() noexcept
{
  return this->registry;
}

renderer::Window& Application::getWindow() noexcept
{
  return *this->window;
}

Application::Application(
    std::string_view title, ApplicationArguments pargs) noexcept
  : args{pargs}
{
  auto exp = renderer::Window::create(title);
  if (!exp)
    std::cerr << exp.error() << '\n';
  else
    this->window = std::move(*exp);
}
} // namespace redoom
