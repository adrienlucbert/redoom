#pragma once

namespace redoom
{
class Layer
{
public:
  Layer() noexcept = default;
  Layer(Layer const&) noexcept = delete;
  Layer(Layer&&) noexcept = default;
  virtual ~Layer() noexcept = default;

  Layer& operator=(Layer const&) noexcept = delete;
  Layer& operator=(Layer&&) noexcept = default;

  virtual void onAttach() noexcept;

  virtual void onDetach() noexcept;

  virtual void onUpdate(double elapsed_time) noexcept;
};
} // namespace redoom
