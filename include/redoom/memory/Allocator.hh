#pragma once

#include <utility>

#include <redoom/memory/IAllocator.hh>

namespace redoom::memory
{
template <typename T>
class Allocator : public IAllocator
{
public:
  Allocator() noexcept = default;
  Allocator(Allocator const& b) noexcept = delete;
  Allocator(Allocator&& b) noexcept = delete;
  ~Allocator() noexcept override = default;

  Allocator& operator=(Allocator const& rhs) noexcept = delete;
  Allocator& operator=(Allocator&& rhs) noexcept = delete;

  void* get() noexcept override
  {
    return this->make_object();
  }
  template <typename... Args>
  void* get(Args&&... args) noexcept
  {
    return this->make_object(std::forward<Args>(args)...);
  }

  void release(void* object) noexcept override
  {
    delete static_cast<T*>(object); // NOLINT(cppcoreguidelines-owning-memory)
  }

private:
  template <typename... Args>
  void* make_object(Args&&... args) noexcept
  {
    return static_cast<void*>(new T(std::forward<Args>(args)...));
  }
};
} // namespace redoom::memory
