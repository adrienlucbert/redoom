#pragma once

namespace redoom::memory
{
class AllocatorBase
{
public:
  AllocatorBase(AllocatorBase const&) noexcept = delete;
  AllocatorBase(AllocatorBase&&) noexcept = default;
  virtual ~AllocatorBase() noexcept = default;

  AllocatorBase& operator=(AllocatorBase const&) noexcept = delete;
  AllocatorBase& operator=(AllocatorBase&&) noexcept = default;

protected:
  AllocatorBase() noexcept = default;
};
} // namespace redoom::memory
