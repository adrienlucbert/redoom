#include <redoom/Application.hh>

#include <cassert>
#include <iostream>

#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include <redoom/graphics/ShaderLibrary.hh>
#include <redoom/renderer/Renderer.hh>
#include <redoom/renderer/Window.hh>
#include <redoom/renderer/pipelines/DefaultRenderPipeline.hh>
#include <redoom/serializer/SceneSerializer.hh>

namespace redoom
{
bool Application::isReady() const noexcept
{
  return static_cast<bool>(this->window_);
}

void Application::run() noexcept
{
  while (glfwWindowShouldClose(
             static_cast<GLFWwindow*>(this->window_->getNativeWindow()))
         == GL_FALSE) {
    auto current_time = glfwGetTime();
    auto const elapsed_time = current_time - this->previous_time_;
    this->previous_time_ = current_time;

    glfwPollEvents();

    // TODO(alucbert): turn this call into a layer
    this->getCurrentScene().getRegistry().update(*this->window_, elapsed_time);

    for (auto const& layer : this->layers_)
      layer->onUpdate(elapsed_time);

    this->window_->onUpdate();
  }
}

void Application::onEvent(events::Event const& event) noexcept
{
  this->events_.push(event);
}

bool Application::pollEvent(events::Event& buffer) noexcept
{
  if (this->events_.empty())
    return false;
  buffer = this->events_.pop();
  return true;
}

void Application::pushLayer(std::shared_ptr<Layer> layer) noexcept
{
  layer->onAttach();
  this->layers_.emplace_back(std::move(layer));
}

void Application::popLayer(std::shared_ptr<Layer> const& layer) noexcept
{
  auto it = std::find(this->layers_.begin(), this->layers_.end(), layer);
  if (it != this->layers_.end()) {
    (*it)->onDetach();
    this->layers_.erase(it);
  }
}

Scene& Application::makeScene(std::string_view name, bool set_current) noexcept
{
  auto [it, success] =
      this->scenes_.emplace(name, std::make_shared<Scene>(name));
  assert(success);
  if (set_current)
    this->current_scene_ = it->second;
  return *it->second;
}

Expected<std::reference_wrapper<Scene>> Application::loadScene(
    std::string_view filepath, bool set_current) noexcept
{
  auto scene = std::make_shared<Scene>("default");
  auto exp = SceneSerializer::get().deserialize(*scene, filepath);
  RETURN_IF_UNEXPECTED(exp);
  auto [it, success] =
      this->scenes_.emplace(scene->getName(), std::move(scene));
  if (!success)
    return tl::make_unexpected("Could not append scene to the application");
  if (set_current)
    this->current_scene_ = it->second;
  return *it->second;
}

Scene& Application::getCurrentScene() noexcept
{
  assert(this->current_scene_ != nullptr && "No current scene set");
  return *this->current_scene_;
}

void Application::setCurrentScene(std::string const& name) noexcept
{
  auto it = this->scenes_.find(name);
  assert(it != this->scenes_.end() && "No scene by this name");
  this->current_scene_ = it->second;
}

renderer::Window& Application::getWindow() noexcept
{
  return *this->window_;
}

ApplicationArguments const& Application::getArgs() const noexcept
{
  return this->args_;
}

Expected<> Application::init(std::string_view title) noexcept
{
  auto window_exp = renderer::Window::create(
      title, 1600, 900, [this](events::Event const& e) { this->onEvent(e); });
  RETURN_IF_UNEXPECTED(window_exp);
  this->window_ = std::move(*window_exp);

  auto mouse_picking_shader_exp =
      graphics::ShaderLibrary::addShader("mouse_picking",
          "../src/shaders/editor/mouse_picking/mouse_picking_vs.glslx",
          "../src/shaders/editor/mouse_picking/mouse_picking_fs.glslx");
  if (!mouse_picking_shader_exp) {
    std::cerr << mouse_picking_shader_exp.error() << '\n';
    std::abort();
  }

  auto draw_framebuffer_shader_exp =
      graphics::ShaderLibrary::addShader("draw_framebuffer",
          "../src/shaders/editor/draw_framebuffer/draw_framebuffer_vs.glslx",
          "../src/shaders/editor/draw_framebuffer/draw_framebuffer_fs.glslx");
  if (!draw_framebuffer_shader_exp) {
    std::cerr << draw_framebuffer_shader_exp.error() << '\n';
    std::abort();
  }

  auto renderer_api_exp = renderer::RendererAPI::create();
  RETURN_IF_UNEXPECTED(renderer_api_exp);
  auto rp = std::make_unique<renderer::pipelines::DefaultRenderPipeline>();
  renderer::Renderer::create(std::move(*renderer_api_exp), std::move(rp));
  return {};
}

Application::Application(
    std::string_view title, ApplicationArguments args) noexcept
  : args_{args}
{
  auto exp = this->init(title);
  if (!exp)
    std::cerr << exp.error() << '\n';
}

Application::~Application() noexcept
{
  for (auto& layer : this->layers_)
    layer->onDetach();
}
} // namespace redoom
