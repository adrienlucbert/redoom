#pragma once

namespace redoom::memory
{
class IAllocator
{
public:
  IAllocator() noexcept = default;
  IAllocator(IAllocator const& b) noexcept = delete;
  IAllocator(IAllocator&& b) noexcept = delete;
  virtual ~IAllocator() noexcept;

  IAllocator& operator=(IAllocator const& rhs) noexcept = delete;
  IAllocator& operator=(IAllocator&& rhs) noexcept = delete;

  virtual void* get() noexcept = 0;
  virtual void release(void* object) noexcept = 0;
};
} // namespace redoom::memory
