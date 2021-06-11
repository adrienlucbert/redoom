#pragma once

#include <functional>
#include <memory>

#include <redoom/memory/AllocatorBase.hh>

namespace redoom::memory
{
template <typename T>
class Allocator : public AllocatorBase
{
public:
  using ptr_t = std::unique_ptr<T, std::function<void(void*)>>;

  Allocator() noexcept = default;
  Allocator(Allocator const& b) noexcept = delete;
  Allocator(Allocator&& b) noexcept = delete;
  ~Allocator() noexcept override = default;

  Allocator& operator=(Allocator const& rhs) noexcept = delete;
  Allocator& operator=(Allocator&& rhs) noexcept = delete;

  template <typename... Args>
  ptr_t get(Args&&... args) noexcept
  {
    return ptr_t{new T(std::forward<Args>(args)...),
        [this](void* ptr) { this->release(static_cast<T*>(ptr)); }};
  }

private:
  void release(T* object) noexcept
  {
    delete static_cast<T*>(object); // NOLINT(cppcoreguidelines-owning-memory)
  }
};
} // namespace redoom::memory
