#pragma once

namespace redoom::memory
{
class AllocatorBase
{
public:
  AllocatorBase(AllocatorBase const& b) noexcept = delete;
  AllocatorBase(AllocatorBase&& b) noexcept = delete;
  virtual ~AllocatorBase() noexcept;

  AllocatorBase& operator=(AllocatorBase const& rhs) noexcept = delete;
  AllocatorBase& operator=(AllocatorBase&& rhs) noexcept = delete;

protected:
  AllocatorBase() noexcept = default;
};
} // namespace redoom::memory
