#pragma once

#include <cassert>
#include <functional>

namespace redoom::memory
{
template <typename T, typename Deleter>
class Ptr
{
public:
  Ptr(T* ptr, Deleter deleter) noexcept
    : ptr_{ptr}
    , deleter_{deleter}
  {
  }
  ~Ptr() noexcept
  {
    this->release();
  }
  Ptr(Ptr const& rhs) noexcept = delete;
  Ptr(Ptr&& rhs) noexcept
    : ptr_{std::move(rhs.ptr_)}
    , deleter_{std::move(rhs.deleter_)}
  {
    rhs.ptr_ = nullptr;
  }

  Ptr& operator=(Ptr const& rhs) noexcept = delete;
  Ptr& operator=(Ptr&& rhs) noexcept
  {
    this->release();
    this->ptr_ = std::move(rhs.ptr_);
    rhs.ptr_ = nullptr;
    this->deleter_ = std::move(rhs.deleter_);
    return *this;
  }
  bool operator==(Ptr const& rhs) const noexcept
  {
    return this->ptr_ == rhs.ptr_;
  }
  bool operator!=(Ptr const& rhs) const noexcept
  {
    return !(*this == rhs);
  }
  T& operator*() noexcept
  {
    return *this->ptr_;
  }
  T* operator->() noexcept
  {
    return this->ptr_;
  }
  T const& operator*() const noexcept
  {
    return *this->ptr_;
  }
  T const* operator->() const noexcept
  {
    return this->ptr_;
  }
  template <typename U>
  operator Ptr<U, Deleter>() noexcept // NOLINT
  {
    auto res = Ptr<U, Deleter>{static_cast<U*>(this->ptr_), this->deleter_};
    this->ptr_ = nullptr;
    return res;
  }

  T* get() const noexcept
  {
    return this->ptr_;
  }

private:
  void release()
  {
    if (this->ptr_ == nullptr)
      return;
    std::invoke(this->deleter_, this->ptr_);
  }

  T* ptr_;
  Deleter deleter_;
};
} // namespace redoom::memory
