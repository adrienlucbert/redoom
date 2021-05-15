#pragma once

namespace redoom::memory
{
class IAllocator
{
public:
  IAllocator() noexcept = default;
  IAllocator(IAllocator const& b) noexcept = default;
  IAllocator(IAllocator&& b) noexcept = default;
  virtual ~IAllocator() noexcept;

  IAllocator& operator=(IAllocator const& rhs) noexcept = default;
  IAllocator& operator=(IAllocator&& rhs) noexcept = default;

  virtual void* get() noexcept = 0;
  virtual void release(void* object) noexcept = 0;
};
} // namespace redoom::memory
