#pragma once

#include <redoom/memory/IAllocator.hh>

namespace redoom::memory
{
template <typename T>
class Allocator : public IAllocator
{
public:
  Allocator() noexcept = default;
  Allocator(Allocator const& b) noexcept = default;
  Allocator(Allocator&& b) noexcept = default;
  ~Allocator() noexcept override = default;

  Allocator& operator=(Allocator const& rhs) noexcept = default;
  Allocator& operator=(Allocator&& rhs) noexcept = default;

  void* get() noexcept override
  {
    return static_cast<void*>(new T());
  }
  void release(void* object) noexcept override
  {
    delete static_cast<T*>(object); // NOLINT(cppcoreguidelines-owning-memory)
  }
};
} // namespace redoom::memory
