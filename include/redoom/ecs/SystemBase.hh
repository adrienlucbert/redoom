#pragma once

#include <redoom/ecs/UpdateContext.hh>

namespace redoom::ecs
{
class SystemBase
{
public:
  SystemBase(SystemBase const& b) noexcept = default;
  SystemBase(SystemBase&& b) noexcept = default;
  virtual ~SystemBase() noexcept = default;

  SystemBase& operator=(SystemBase const& rhs) noexcept = default;
  SystemBase& operator=(SystemBase&& rhs) noexcept = default;

  virtual void update(UpdateContext& /*unused*/) noexcept;

  [[nodiscard]] bool isMultithreaded() const noexcept;

protected:
  explicit SystemBase(bool is_multithreaded) noexcept;

  bool is_multithreaded_;
};
} // namespace redoom::ecs
