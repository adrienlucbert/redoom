#pragma once

namespace redoom::memory
{
class AllocatorBase
{
public:
  AllocatorBase(AllocatorBase const& b) noexcept = delete;
  AllocatorBase(AllocatorBase&& b) noexcept = default;
  virtual ~AllocatorBase() noexcept = default;

  AllocatorBase& operator=(AllocatorBase const& rhs) noexcept = delete;
  AllocatorBase& operator=(AllocatorBase&& rhs) noexcept = default;

protected:
  AllocatorBase() noexcept = default;
};
} // namespace redoom::memory
