#include <redoom/Application.hh>

#include <cassert>
#include <iostream>

#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include <redoom/SceneSerializer.hh>
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

    this->getCurrentScene().getRegistry().update(*this->window, elapsed_time);

    this->window->onUpdate();
  }
}

Scene& Application::makeScene(std::string_view name, bool set_current) noexcept
{
  auto [it, success] =
      this->scenes.emplace(name, std::make_shared<Scene>(name));
  assert(success);
  if (set_current)
    this->current_scene = it->second;
  return *it->second;
}

Expected<std::reference_wrapper<Scene>> Application::loadScene(
    std::string_view filepath, bool set_current) noexcept
{
  auto scene = std::make_shared<Scene>("default");
  auto exp = SceneSerializer::get().deserialize(*scene, filepath);
  RETURN_IF_UNEXPECTED(exp);
  auto [it, success] = this->scenes.emplace(scene->getName(), std::move(scene));
  if (!success)
    return tl::make_unexpected("Could not append scene to the application");
  if (set_current)
    this->current_scene = it->second;
  return *it->second;
}

Scene& Application::getCurrentScene() noexcept
{
  assert(this->current_scene != nullptr && "No current scene set");
  return *this->current_scene;
}

void Application::setCurrentScene(std::string const& name) noexcept
{
  auto it = this->scenes.find(name);
  assert(it != this->scenes.end() && "No scene by this name");
  this->current_scene = it->second;
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
