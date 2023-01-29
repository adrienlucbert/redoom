#pragma once

#include <redoom/ecs/UpdateContext.hh>

namespace redoom::ecs
{
class SystemBase
{
public:
  SystemBase(SystemBase const&) noexcept = default;
  SystemBase(SystemBase&&) noexcept = default;
  virtual ~SystemBase() noexcept = default;

  SystemBase& operator=(SystemBase const&) noexcept = default;
  SystemBase& operator=(SystemBase&&) noexcept = default;

  virtual void onAttach() noexcept;

  virtual void beforeUpdate(UpdateContext& /*unused*/) noexcept;
  virtual void update(UpdateContext& /*unused*/) noexcept;
  virtual void afterUpdate(UpdateContext& /*unused*/) noexcept;

  virtual void onDetach() noexcept;

  [[nodiscard]] bool isMultithreaded() const noexcept;

protected:
  explicit SystemBase(bool is_multithreaded) noexcept;

  bool is_multithreaded_;
};
} // namespace redoom::ecs
