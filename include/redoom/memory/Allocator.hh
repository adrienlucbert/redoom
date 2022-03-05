#pragma once

#include <functional>
#include <memory>
#include <type_traits>

#include <redoom/memory/AllocatorBase.hh>
#include <redoom/memory/ObjectPool.hh>
#include <redoom/memory/Ptr.hh>

namespace redoom::memory
{
template <typename T>
class Allocator : public AllocatorBase
{
public:
  using ptr_t = Ptr<T, std::function<void(void*)>>;

  friend ptr_t;

  Allocator() noexcept = default;
  Allocator(Allocator const& b) noexcept = delete;
  Allocator(Allocator&& b) noexcept = default;
  ~Allocator() noexcept override = default;

  Allocator& operator=(Allocator const& rhs) noexcept = delete;
  Allocator& operator=(Allocator&& rhs) noexcept = default;

  template <typename... Args>
  ptr_t get(Args&&... args) noexcept
  {
    return ptr_t{this->pool_.get(std::forward<Args>(args)...),
        [this](void* ptr) { this->release(ptr); }};
  }

private:
  void release(void* object) noexcept
  {
    this->pool_.release(static_cast<T*>(object));
  }

  ObjectPool<T> pool_;
};
} // namespace redoom::memory
