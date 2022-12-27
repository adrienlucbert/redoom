#include <redoom/Application.hh>

#include <redoom/layers/ImGuiLayer.hh>
#include <redoom/layers/RuntimeLayer.hh>

#include <redoom/Runtime.hh>

namespace redoom
{
bool Application::isReady() const noexcept
{
  return Runtime::get().isReady();
}

void Application::run() noexcept
{
  while (!Runtime::get().shouldClose()) {
    auto current_time = Runtime::get().getTime();
    auto const elapsed_time = current_time - this->previous_time_;
    this->previous_time_ = current_time;

    for (auto const& layer : this->layers_)
      layer->onUpdate(elapsed_time);

    for (auto const& layer : this->layers_)
      layer->afterUpdate();
  }
}

void Application::onEvent(events::Event const& event) noexcept
{
  // NOLINTNEXTLINE(modernize-loop-convert) because std::range is not ready yet.
  for (auto it = this->layers_.rbegin(); it != this->layers_.rend(); ++it) {
    // Pass events through layers (last to first) unless one halts propagation.
    if ((*it)->onEvent(event) == EventPropagation::Halt)
      break;
  }
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

Runtime& Application::getRuntime() noexcept
{
  return this->runtime_;
}

ApplicationArguments const& Application::getArgs() const noexcept
{
  return this->args_;
}

Application::Application(
    std::string_view title, ApplicationArguments args) noexcept
  : args_{args}
  , layers_{std::make_shared<RuntimeLayer>()}
  , runtime_{title, [this](events::Event const& e) { this->onEvent(e); }}
{
  for (auto const& layer : this->layers_)
    layer->onAttach();
}

Application::~Application() noexcept
{
  for (auto& layer : this->layers_)
    layer->onDetach();
}
} // namespace redoom
