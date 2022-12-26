#include <redoom/Runtime.hh>

#include <cassert>
#include <string_view>

#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include <redoom/Application.hh>
#include <redoom/graphics/ShaderLibrary.hh>
#include <redoom/renderer/Renderer.hh>
#include <redoom/renderer/pipelines/DefaultRenderPipeline.hh>
#include <redoom/serializer/SceneSerializer.hh>

namespace redoom
{
Runtime& Runtime::get() noexcept
{
  return Application::get().getRuntime();
}

bool Runtime::shouldClose() const noexcept
{
  return glfwWindowShouldClose(
             static_cast<GLFWwindow*>(this->window_->getNativeWindow()))
      == GL_TRUE;
}

double Runtime::getTime() const noexcept
{
  return glfwGetTime();
}

bool Runtime::isReady() const noexcept
{
  return static_cast<bool>(this->window_);
}

void Runtime::onUpdate(double elapsed_time) noexcept
{
  glfwPollEvents();
  this->getCurrentScene().getRegistry().update(*this->window_, elapsed_time);
}

void Runtime::afterUpdate() noexcept
{
  this->window_->onUpdate();
}

void Runtime::onEvent(events::Event const& event) noexcept
{
  this->events_.push(event);
}

bool Runtime::pollEvent(events::Event& buffer) noexcept
{
  if (this->events_.empty())
    return false;
  buffer = this->events_.pop();
  return true;
}

Scene& Runtime::makeScene(std::string_view name, bool set_current) noexcept
{
  auto [it, success] =
      this->scenes_.emplace(name, std::make_shared<Scene>(name));
  assert(success);
  if (set_current)
    this->current_scene_ = it->second;
  return *it->second;
}

Expected<std::reference_wrapper<Scene>> Runtime::loadScene(
    std::string_view filepath, bool set_current) noexcept
{
  auto scene = std::make_shared<Scene>("default");
  auto exp = SceneSerializer::get().deserialize(*scene, filepath);
  RETURN_IF_UNEXPECTED(exp);
  auto [it, success] =
      this->scenes_.emplace(scene->getName(), std::move(scene));
  if (!success)
    return tl::make_unexpected("Could not append scene to the runtime");
  if (set_current)
    this->current_scene_ = it->second;
  return *it->second;
}

Scene& Runtime::getCurrentScene() noexcept
{
  assert(this->current_scene_ != nullptr && "No current scene set");
  return *this->current_scene_;
}

void Runtime::setCurrentScene(std::string const& name) noexcept
{
  auto it = this->scenes_.find(name);
  assert(it != this->scenes_.end() && "No scene by this name");
  this->current_scene_ = it->second;
}

renderer::Window& Runtime::getWindow() noexcept
{
  return *this->window_;
}

Expected<> Runtime::init(std::string_view title,
    renderer::EventCallback const& event_callback) noexcept
{
  auto window_exp = renderer::Window::create(title, 1600, 900, event_callback);
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

Runtime::Runtime(std::string_view title,
    renderer::EventCallback const& event_callback) noexcept
{
  auto exp = this->init(title, event_callback);
  if (!exp)
    std::cerr << exp.error() << '\n';
}
} // namespace redoom
